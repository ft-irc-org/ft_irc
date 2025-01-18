#include "Part.hpp"

Part::Part() {
}

Part::~Part() {
}

void Part::execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels) {
	if (command.getParamCount() < 1) {
		std::string response = ":localhost 461 " + client->getNickname() + " PART :Not enough parameters\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
		return;
	}

	std::string channelName = command.getParam(0);
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		std::string response = ":localhost 403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
		return;
	}

	Channel* channel = channels[channelName];
	channel->removeMember(*client);

	std::string response = ":" + client->getNickname() + " PART " + channelName + "\r\n";
	send(client->getSocketFd(), response.c_str(), response.size(), 0);
}