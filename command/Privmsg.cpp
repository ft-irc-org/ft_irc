#include "Privmsg.hpp"

Privmsg::Privmsg() {
}

Privmsg::~Privmsg() {
}

void Privmsg::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
	(void) clients;
	(void) auth;
	(void) server;
	if (isParamCountValid(sender, command, server, 2, "461", " PRIVMSG :Not enough parameters\r\n") == false) {
		return;
	}

	std::string target = command.getParam(0);
	std::string message = command.getParam(1);

	if (target[0] == '#') {
		(void) auth;
		std::map<std::string, Channel*>::iterator it = channels.find(target);
		if (validateChannelExists(sender, channels, server, target, "403", " :No such channel\r\n") == false) {
			return;
		}

		Channel* channel = it->second;
		// if (!auth.hasPermission(sender->getNickname(), target, Auth::CHANNEL_MODE)) {
		// 	std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + " " + target + " :You're not channel operator :: PRIVMSG\r\n";
		// 	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		// 	return;
		// }

		std::string response = message + "\r\n";
		channel->broadcast(response, sender);
	} else {
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			if (it->first == sender->getSocketFd()) {
				continue;
			}

			if (it->second->getNickname() == target) {
				std::string response = "*" + sender->getNickname() + "* " + message + "\r\n";
				// send(it->first, response.c_str(), response.size(), 0);
				it->second->setOutBuffer(response);
				return;
			}
		}
	}
}