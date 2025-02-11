#include "Notice.hpp"

Notice::Notice() {
}

Notice::~Notice() {
}

void Notice::execute(Client* sender, const Message& command, 
				  std::map<int, Client*> &clients, 
				  std::map<std::string, Channel*>& channels, 
				  Auth &auth, ServerEventHandler *server) {
	(void)channels;
	(void)auth;
	(void)server;

	if (command.getParamCount() < 2) {
		std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + 
							 " NOTICE :Not enough parameters\r\n";
		sender->setOutBuffer(response);
		return;
	}

	std::string target = command.getParam(0);
	std::string message = command.getParam(1);

	if (target[0] == '#') {
		std::string response = ":" + server->getServerName() + " 401 " + sender->getNickname() + 
							 " " + target + " :No such nick/channel\r\n";
		sender->setOutBuffer(response);
		return;
	}

	std::map<int, Client*>::iterator it = clients.begin();
	while (it != clients.end()) {
		if (it->second->getNickname() == target) {
			std::string response = ":" + sender->getNickname() + " NOTICE " + target + " :" + message + "\r\n";
			it->second->setOutBuffer(response);
			return;
		}
		it++;
	}

	std::string response = ":" + server->getServerName() + " 401 " + sender->getNickname() + 
						 " " + target + " :No such nick/channel\r\n";
	sender->setOutBuffer(response);
}