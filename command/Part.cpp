#include "Part.hpp"

Part::Part() {
}

Part::~Part() {
}

bool Part::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

bool Part::verifyChannelSyntax(Client* sender, ServerEventHandler *server, const std::string& channelName, const std::string& errorMessage) {
    if (channelName[0] != '#') {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
        return false;
    }
	return true;
}

bool Part::validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
		return;
	}
}

void Part::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) server;
    (void) auth; // 나갈 때 권한 위임하거나 해제하는 것은 없음

    if (isParamCountValid(sender, command, server, 1, " PART :Not enough parameters\r\n") == false) {
        return;
    }

    std::string channelName = command.getParam(0);
    std::string nickname = sender->getNickname();

    if (verifyChannelSyntax(sender, server, channelName, " :No such channel\r\n")) {
        return;
    }

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (validateChannelExists(sender, channels, server, channelName, " :No such channel\r\n")) {
        return;
    }

    Channel* channel = it->second;
    
    if (!channel->isMember(sender)) {
        sendError(sender, ":" + server->getServerName() + " 442 " + nickname + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

	// 3. 전체 포맷 재구성
	std::string source = nickname + "!" + sender->getNickname() + "@" + server->getServerName() + "";
    std::string partMessage = ":" + source + " PART :" + channelName + "\r\n";  
    
    // 파트하는 클라이언트에게 먼저 PART 메시지 전송
    sender->setOutBuffer(partMessage);

	channel->broadcast(partMessage, sender);

	// 채널에서 유저 제거 (이 시점에서 실제로 제거)
    channel->removeMember(sender);

    // channel->broadcast(partMessage, sender);

    // 채널이 비었으면 삭제
    if (channel->getUserCount() == 0) {
        channels.erase(channelName);
        delete channel;
        return;  // 채널이 삭제되었으므로 여기서 종료
    }
}