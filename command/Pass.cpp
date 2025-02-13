#include "Pass.hpp"

Pass::Pass() {
}

Pass::~Pass() {
}

void Pass::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void) channels;
	(void) clients;

	if (command.getParamCount() < 1) {
		// std::cout << "command.getParamCount() < 1" << std::endl;
		sender->setOutBuffer(":" + server->getServerName() + " 461 " + sender->getNickname() + " PASS :Not enough parameters\r\n");
		return;
	}

	if (sender->isPassAuthenticated()) {
		// std::cout << "sender->isPassAuthenticated()" << std::endl;
		// sender->setOutBuffer(":" + server->getServerName() + " 462 " + sender->getNickname() + " :You may not reregister\r\n");
		return;
	}

	std::string password = command.getParam(0);

	if (password != auth.getPassword()) {
		sender->setOutBuffer(":" + server->getServerName() + " 464 " + sender->getNickname() + " :Password incorrect\r\n");
		disconnectClient(sender, server, clients);
		return;
	}

	sender->setPassAuthenticated(true);
}