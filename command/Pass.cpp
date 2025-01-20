#include "Pass.hpp"

Pass::Pass() {
}

Pass::~Pass() {
}

void Pass::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth){
	(void) channels;
	(void) clients;
	if (command.getParamCount() < 1) {
		sendError(sender, "461 " + sender->getNickname() + " PASS :Not enough parameters");
		return;
	}

	std::string password = command.getParam(0);

	if (password != auth.getPassword()) {
		sendError(sender, "464 " + sender->getNickname() + " :Password incorrect");
		return;
	}

	sender->setAuthentication(true);
	std::string response = ":localhost 001 " + sender->getNickname() + " :Password accepted\r\n";
	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
}