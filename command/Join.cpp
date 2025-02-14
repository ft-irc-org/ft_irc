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
        sender->setOutBuffer(response);
        return;
    }

    std::string channelName = command.getParam(0);
    std::string password = command.getParamCount() > 1 ? command.getParam(1) : "";

    if (channelName.length() == 1 && channelName[0] == '0') {
        std::vector<Channel*> userChannels = auth.getChannels(sender->getNickname());
        for (std::vector<Channel*>::iterator it = userChannels.begin(); it != userChannels.end(); ++it) {
            Channel* channel = *it;
            std::string response = ":" + sender->getNickname() + "!~" + 
                      sender->getRealname() + "@" + 
                      sender->getIp() + " PART " + channel->getChannelName() + "\r\n";
            channel->broadcast(response, sender, server);
            sender->setOutBuffer(response);
            if (channel->isMember(sender))
                channel->removeMember(sender);
            if (channel->isWhiteList(sender))
                channel->removeWhiteList(sender);
        }
        return;
    }

    if (channelName[0] != '#') {
        std::string response = ":" + server->getServerName() + " 476 " + sender->getNickname() + 
                             " " + channelName + " :Bad Channel Mask\r\n";
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

        // 이미 채널에 참가 중인 경우
        if (channel->isMember(sender)) {
            return;
        }


        // 비밀번호 체크
        if (!channel->getPassword().empty()) {
            if (password != channel->getPassword()) {
                std::string response = ":" + server->getServerName() + " 475 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+k)\r\n";
                sender->setOutBuffer(response);
                return;
            }
        }

        // user limit 체크
        if (channel->getChannelMode() & Channel::USER_LIMIT) {
            if (channel->getUserCount() >= channel->getUserLimit()) {
                std::string response = ":" + server->getServerName() + " 471 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+l)\r\n";
                sender->setOutBuffer(response);
                return;
            }
        }

        if (channel->getChannelMode() & Channel::INVITE_ONLY) {
            if (!channel->isWhiteList(sender)) {
                std::string response = ":" + server->getServerName() + " 473 " + sender->getNickname() + 
                                     " " + channelName + " :Cannot join channel (+i)\r\n";
                sender->setOutBuffer(response);
                return;
            }
        }
    }
    
    // 채널 참가
    channel->addMember(sender);
    //std::string response = ":" + sender->getNickname() + " " + command + " " + channelName + " :" + message + "\r\n";
    // std::string response = ":" + sender->getNickname() + " JOIN " + channelName + "\r\n";

    std::string response = ":" + sender->getNickname() + "!~" + 
                      sender->getRealname() + "@" + 
                      sender->getIp() + " JOIN " + 
                      channelName + "\r\n";


    channel->broadcast(response, sender, server);
    sender->setOutBuffer(response);
	// 기존 권한이 없을 때만 NONE 권한 추가
    if (auth.isNoob(sender->getNickname())) {
        auth.grantPermission(sender->getNickname(), channelName, Auth::NONE);
    }
    sender->setCurrentChannel(channelName);

    // Topic 정보 전송
    if (!channel->getTopic().empty()) {
        // topic이 있을 때 RPL_TOPIC (332) 전송
        response = ":" + server->getServerName() + " 332 " + 
                sender->getNickname() + " " + channelName + 
                " :" + channel->getTopic() + "\r\n";
        sender->setOutBuffer(response);
        
        // RPL_TOPICWHOTIME (333) 전송
        std::stringstream ss;
        ss << channel->getTopicTime();
        std::string whoTimeMsg = ":" + server->getServerName() + " 333 " + 
                                sender->getNickname() + " " + channelName + " " +
                                channel->getTopicSetter() + " " + 
                                ss.str() + "\r\n";
        sender->setOutBuffer(whoTimeMsg);
    }
    else {
        // topic이 없을 때 RPL_NOTOPIC (331) 전송
        response = ":" + server->getServerName() + " 331 " + 
                sender->getNickname() + " " + channelName + 
                " :No topic is set\r\n";
        sender->setOutBuffer(response);
    }

    // NAMEREPLY 전송
    std::string nameReply = ":" + server->getServerName() + " 353 " + 
                          sender->getNickname() + " = " + channelName + " :";
    std::map<int, Client*> users = channel->getUsers();
    for (std::map<int, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
        std::string prefix = "";
        if (auth.isOperator(it->second->getNickname(), channelName)) {
            prefix = "@";  // 채널 운영자
        }
        nameReply += prefix + it->second->getNickname() + " ";
    }
    nameReply += "\r\n";
    sender->setOutBuffer(nameReply);

    // END OF NAMEREPLY 전송
    response = ":" + server->getServerName() + " 366 " + 
            sender->getNickname() + " " + channelName + 
            " :End of /NAMES list\r\n";
    sender->setOutBuffer(response);
}