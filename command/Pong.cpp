#include "Pong.hpp"

Pong::Pong() {
}

Pong::~Pong() {
}

void Pong::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
	(void) clients;
	(void) channels;
	(void) auth;
	if (isParamCountValid(sender, command, server, 1, "461", " PONG :Not enough parameters\r\n") == false) {
        return;
    }

	std::string response = ":" + server->getServerName() + " PONG " + server->getServerName() + " " + command.getParam(0) + "\r\n";
	// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	sender->setOutBuffer(response);
}