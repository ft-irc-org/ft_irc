#include "Part.hpp"

Part::Part() {
}

Part::~Part() {
}

void Part::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	(void) clients;
	if (command.getParamCount() < 1) {
		return sendError(sender, "461 PART :Not enough parameters");
	}

	std::string channelName = command.getParam(0);
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		return sendError(sender, "403 " + channelName + " :No such channel");
	}

	Channel* channel = it->second;
	if (channel->getChannelMode() & Channel::OPERATOR_ONLY && !auth.isOperator(sender->getNickname(), channelName)) {
		return sendError(sender, "482 " + channelName + " :You're not channel operator :: PART");
	}

	channel->removeMember(*sender);
	std::string response = ":" + sender->getNickname() + " PART " + channelName + "\r\n";
	channel->broadcast(response, *sender);
	if (channel->getUserCount() == 0) {
		channels.erase(channelName);
		delete channel;
	}
}