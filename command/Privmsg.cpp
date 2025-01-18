#include "Privmsg.hpp"

Privmsg::Privmsg() {
}

Privmsg::~Privmsg() {
}

void Privmsg::execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels) {
	if (command.getParamCount() < 2) {
		std::string response = ":localhost 461 " + client->getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
		return;
	}

	std::string target = command.getParam(0);
	std::string message = command.getParam(1);

	if (target[0] == '#') {
		std::map<std::string, Channel*>::iterator it = channels.find(target);
		if (it == channels.end()) {
			std::string response = ":localhost 403 " + client->getNickname() + " " + target + " :No such channel\r\n";
			send(client->getSocketFd(), response.c_str(), response.size(), 0);
			return;
		}

		Channel* channel = channels[target];
		channel->broadcast(message, *client);
	} else {
		std::string response = ":" + client->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
	}
}