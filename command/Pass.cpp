#include "Pass.hpp"

Pass::Pass() {
}

Pass::~Pass() {
}

void Pass::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void) channels;
	(void) clients;

	if (isParamCountValid(sender, command, server, 1, "461", " PASS :Not enough parameters") == false) {
		return;
	}

	if (sender->isPassAuthenticated()) {
		// std::cout << "sender->isPassAuthenticated()" << std::endl;
		sendError(sender, "462 " + sender->getNickname() + " :You may not reregister");
		return;
	}

	std::string password = command.getParam(0);

	if (password != auth.getPassword()) {
		sendError(sender, "464 " + sender->getNickname() + " :Password incorrect");
		disconnectClient(sender, server, clients);
		return;
	}

	sender->setPassAuthenticated(true);
}