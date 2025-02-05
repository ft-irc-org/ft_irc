#include "Whois.hpp"

Whois::Whois() {
}

Whois::~Whois() {
}

bool Whois::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

void Whois::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void)channels;
	(void)auth;

	if (isParamCountValid(sender, command, server, 2, " :No nickname given") == false) {
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

	std::string response = ":" + server->getServerName() + " 311 " + sender->getNickname() + " " + target->getNickname() + "\r\n";
	// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	sender->setOutBuffer(response);
	response = ":" + server->getServerName() + " 318 " + sender->getNickname() + " " + target->getNickname() + " :End of /WHOIS list\r\n";
	// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	sender->setOutBuffer(response);
}