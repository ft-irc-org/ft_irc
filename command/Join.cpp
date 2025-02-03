#include "Join.hpp"

Join::Join() {
}

Join::~Join() {
}

bool Join::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

bool Join::verifyChannelSyntax(Client* sender, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
    if (channelName[0] != '#') {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
        return false;
    }
	return true;
}

void Join::execute(Client* sender, const Message& command, 
                  std::map<int, Client*> &clients, 
                  std::map<std::string, Channel*>& channels, 
                  Auth &auth, ServerEventHandler *server) {
    (void)clients;
    (void)server;

    if (isParamCountValid(sender, command, server, 1, " JOIN :Not enough parameters\r\n") == false) {
		return ;
	}

    std::string channelName = command.getParam(0);
    std::string password = command.getParamCount() > 1 ? command.getParam(1) : "";

   	if (verifyChannelSyntax(sender, server, channelName, " :Wrong format : Wrong channel name\r\n") == false)
		return ;

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
    if (!channel->getTopic().empty()) {
        response = ":" + server->getServerName() + " 332 " + sender->getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
    }
}