#include "Join.hpp"

Join::Join() {
}

Join::~Join() {
}

void Join::execute(Client* sender, const Message& command, 
                  std::map<int, Client*> &clients, 
                  std::map<std::string, Channel*>& channels, 
                  Auth &auth) {
    (void)clients;
    
    if (command.getParamCount() < 1) {
        std::string response = ":localhost 461 " + sender->getNickname() + 
                             " JOIN :Not enough parameters\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
        return;
    }

    std::string channelName = command.getParam(0);
    std::string password = command.getParamCount() > 1 ? command.getParam(1) : "";

    if (channelName[0] != '#') {
        std::string response = ":localhost 403 " + sender->getNickname() + 
                             " " + channelName + " :No such channel\r\n";
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
        auth.grantPermission(sender->getNickname(), channelName, 
                           Auth::CHANNEL_MODE | Auth::KICK_POWER | 
                           Auth::INVITE_POWER | Auth::TOPIC_POWER);
    } else {
        channel = it->second;

        // invite-only 모드 체크
        if (channel->getChannelMode() & Channel::INVITE_ONLY) {
            if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
                std::string response = ":localhost 473 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+i)\r\n";
                // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
                sender->setOutBuffer(response);
                return;
            }
        }

        // 비밀번호 체크
        if (!channel->getPassword().empty()) {
            if (password != channel->getPassword()) {
                std::string response = ":localhost 475 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+k)\r\n";
                // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
                sender->setOutBuffer(response);
                return;
            }
        }

        // user limit 체크
        if (channel->getChannelMode() & Channel::USER_LIMIT) {
            if (channel->getUserCount() >= channel->getUserLimit()) {
                std::string response = ":localhost 471 " + sender->getNickname() + 
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
	auth.grantPermission(sender->getNickname(), channelName, Auth::NONE);

    // Topic 정보 전송
    if (!channel->getTopic().empty()) {
        response = ":localhost 332 " + sender->getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
    }
}