#include "Whois.hpp"

Whois::Whois() {
}

Whois::~Whois() {
}

void Whois::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	(void)channels;
	(void)auth;
	if (command.getParamCount() < 2) {
		sendError(sender, "431 " + command.getParam(0) + " :No nickname given");
		return;
	}

	std::string targetNickname = command.getParam(1);
	Client *target = NULL;
	std::map<int, Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second->getNickname() == targetNickname) {
			target = it->second;
			break;
		}
	}

	if (target == NULL) {
		sendError(sender, "401 " + command.getParam(0) + " " + targetNickname + " :No such nick/channel");
		return;
	}

	std::string response = ":localhost 311 " + sender->getNickname() + " " + target->getNickname() + "\r\n";
	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	response = ":localhost 318 " + sender->getNickname() + " " + target->getNickname() + " :End of /WHOIS list\r\n";
	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
}