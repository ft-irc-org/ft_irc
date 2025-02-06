#include "Join.hpp"

Join::Join() {
}

Join::~Join() {
}

void Join::execute(Client* sender, const Message& command, 
                  std::map<int, Client*> &clients, 
                  std::map<std::string, Channel*>& channels, 
                  Auth &auth, ServerEventHandler *server) {
    (void)clients;
    (void)server;
    
    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + 
                             " JOIN :Not enough parameters\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
        return;
    }

    std::string channelName = command.getParam(0);
    std::string password = command.getParamCount() > 1 ? command.getParam(1) : "";

    if (channelName[0] != '#') {
        std::string response = ":" + server->getServerName() + " 403 " + sender->getNickname() + 
                             " " + channelName + " :Wrong format : Wrong channel name\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
        return;
    }

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    Channel* channel;
    
    if (it == channels.end()) {
        // 새로운 채널 생성
        channel = new Channel(-1, channelName); // fd는 -1로 설정하면 자동으로 올라가게
        channels[channelName] = channel;
        auth.grantOperator(sender->getNickname(), channelName); // 첫 유저는 operator
        auth.grantPermission(sender->getNickname(), channelName, Auth::OP);
        channel->setServerName(server->getServerName());
    } else {
        channel = it->second;

        // invite-only 모드 체크
        if (channel->getChannelMode() & Channel::INVITE_ONLY) {
            if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
                std::string response = ":" + server->getServerName() + " 473 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+i)\r\n";
                // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
                sender->setOutBuffer(response);
                return;
            }
        }

        // 비밀번호 체크
        if (!channel->getPassword().empty()) {
            if (password != channel->getPassword()) {
                std::string response = ":" + server->getServerName() + " 475 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+k)\r\n";
                // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
                sender->setOutBuffer(response);
                return;
            }
        }

        // user limit 체크
        if (channel->getChannelMode() & Channel::USER_LIMIT) {
            if (channel->getUserCount() >= channel->getUserLimit()) {
                std::string response = ":" + server->getServerName() + " 471 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+l)\r\n";
                // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
                sender->setOutBuffer(response);
                return;
            }
        }
    }
    
    // 채널 참가
    channel->addMember(sender);
    std::string response = sender->getNickname() + " has joined ";
    channel->broadcast(response, sender);
	// 기존 권한이 없을 때만 NONE 권한 추가
    if (auth.isNoob(sender->getNickname())) {
        auth.grantPermission(sender->getNickname(), channelName, Auth::NONE);
    }
    sender->setCurrentChannel(channelName);

    // Topic 정보 전송
    if (channel->getTopic().empty()) {
        // topic이 없을 때 RPL_NOTOPIC (331) 전송
        response = ":" + server->getServerName() + " 331 " + 
                sender->getNickname() + " " + channelName + 
                " :No topic is set\r\n";
        sender->setOutBuffer(response);
    } else {
        // topic이 있을 때 RPL_TOPIC (332) 전송
        response = ":" + server->getServerName() + " 332 " + 
                sender->getNickname() + " " + channelName + 
                " :" + channel->getTopic() + "\r\n";
        sender->setOutBuffer(response);
        
        // RPL_TOPICWHOTIME (333) 전송
        std::string whoTimeMsg = ":" + server->getServerName() + " 333 " + 
                                sender->getNickname() + " " + channelName + " " +
                                channel->getTopicSetter() + " " + 
                                std::to_string(channel->getTopicTime()) + "\r\n";
        sender->setOutBuffer(whoTimeMsg);
    }
}