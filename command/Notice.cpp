#include "Notice.hpp"

Notice::Notice() {
}

Notice::~Notice() {
}

void Notice::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	(void) command;
	(void) clients;
	(void) channels;
	(void) auth;
	std::string response = ":" + sender->getNickname() + " NOTICE\r\n";
	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
}