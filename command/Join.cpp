#include "Join.hpp"

Join::Join() {
}

Join::~Join() {
}

Channel* Join::createChannel(Client* sender, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server, std::string& channelName) {
	Channel* channel;
	channel = new Channel(-1, channelName); // fd는 -1로 설정하면 자동으로 올라가게
	channels[channelName] = channel;
	auth.grantOperator(sender->getNickname(), channelName); // 첫 유저는 operator
	auth.grantPermission(sender->getNickname(), channelName, Auth::OP);
	channel->setServerName(server->getServerName());
	return channel;
}

bool Join::checkInviteOnlyAccess(Client* sender, Auth &auth, ServerEventHandler *server, const std::string& channelName) {
	if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
		sendError(sender,  ":" + server->getServerName() + " 473 " + sender->getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n");
		return false;
	}
	return true;
}

bool Join::checkChannelPassword(Client* sender, ServerEventHandler *server, Channel* channel, const std::string& password, const std::string& channelName) {
	if (password != channel->getPassword()) {
		sendError(sender, ":" + server->getServerName() + " 475 " + sender->getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n");
		return false;
	}
	return true;
}

bool Join::checkUserLimit(Client* sender, ServerEventHandler *server, Channel* channel, const std::string& channelName) {
	if (channel->getUserCount() >= channel->getUserLimit()) {
		sendError(sender, ":" + server->getServerName() + " 471 " + sender->getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n");
		return false;
	}
	return true;
}

void Join::processChannelJoin(Client* sender, Auth &auth, Channel* channel, const std::string& channelName) {
    channel->addMember(sender);
    std::string response = sender->getNickname() + " has joined ";
    channel->broadcast(response, sender);
	// 기존 권한이 없을 때만 NONE 권한 추가
    if (auth.isNoob(sender->getNickname())) {
        auth.grantPermission(sender->getNickname(), channelName, Auth::NONE);
    }
    sender->setCurrentChannel(channelName);
}

void sendChannelTopic(Client* sender, ServerEventHandler *server, Channel* channel, const std::string& channelName) {
	if (!channel->getTopic().empty()) {
        std::string response = ":" + server->getServerName() + " 332 " + sender->getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.size(), 0);
        sender->setOutBuffer(response);
    }
}

void Join::execute(Client* sender, const Message& command,
                  std::map<int, Client*> &clients,
                  std::map<std::string, Channel*>& channels,
                  Auth &auth, ServerEventHandler *server) {
    (void)clients;

    if (isParamCountValid(sender, command, server, 1, "461", " JOIN :Not enough parameters\r\n") == false) {
		return ;
	}

    std::string channelName = command.getParam(0);
    std::string password = command.getParamCount() > 1 ? command.getParam(1) : "";

   	if (verifyChannelSyntax(sender, server, channelName, "403", " :Wrong format : Wrong channel name\r\n") == false)
		return ;

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    Channel* channel;
    
    if (it == channels.end()) {
        channel = createChannel(sender, channels, auth, server, channelName);
    } else {
        channel = it->second;

        // invite-only 모드 체크
        if (channel->getChannelMode() & Channel::INVITE_ONLY) {
			if (checkInviteOnlyAccess(sender, auth, server, channelName) == false)
				return ;
        }

        // 비밀번호 체크
        if (!channel->getPassword().empty()) {
            if (checkChannelPassword(sender, server, channel, password, channelName) == false)
				return ;
        }

        // user limit 체크
        if (channel->getChannelMode() & Channel::USER_LIMIT) {
            if (checkUserLimit(sender, server, channel, channelName) == false)
				return ;
        }
    }

	// 채널 참가
	processChannelJoin(sender, auth, channel, channelName);

    // Topic 정보 전송
	sendChannelTopic(sender, server, channel, channelName);
    
}