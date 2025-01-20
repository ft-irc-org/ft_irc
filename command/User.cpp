#include "User.hpp"

User::User() {
}

User::~User() {
}

void User::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	(void)clients;
	(void)channels;
	(void)auth;

	if (command.getParamCount() < 4) {
		sendError(sender, "461 " + command.getParam(0) + " USER :Not enough parameters");
		return;
	}

	if (sender->isAuthenticated()) {
		sendError(sender, "462 " + command.getParam(0) + " :You may not reregister");
		return;
	}

	sender->setAuthentication(true);
	sender->setNickname(command.getParam(0));
}