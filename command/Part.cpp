#include "Part.hpp"

Part::Part() {
}

Part::~Part() {
}

void Part::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) server;
    (void) auth; // 나갈 때 권한 위임하거나 해제하는 것은 없음

    if (command.getParamCount() < 1) {
        sendError(sender, ":localhost 461 " + sender->getNickname() + " PART :Not enough parameters\r\n");
        return;
    }

    std::string channelName = command.getParam(0);
    std::string nickname = sender->getNickname();

    if (channelName[0] != '#') {
        sendError(sender, ":localhost 403 " + nickname + " " + channelName + " :No such channel\r\n");
        return;
    }

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        sendError(sender, ":localhost 403 " + nickname + " " + channelName + " :No such channel\r\n");
        return;
    }

    Channel* channel = it->second;
    
    if (!channel->isMember(sender)) {
        sendError(sender, ":localhost 442 " + nickname + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

	// 3. 전체 포맷 재구성
	std::string source = nickname + "!" + sender->getNickname() + "@localhost";
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