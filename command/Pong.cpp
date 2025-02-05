#include "Pong.hpp"

Pong::Pong() {
}

Pong::~Pong() {
}

bool Pong::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

void Pong::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
	(void) clients;
	(void) channels;
	(void) auth;
	if (isParamCountValid(sender, command, server, 1, " PONG :Not enough parameters\r\n") == false) {
        return;
    }

	std::string response = ":" + server->getServerName() + " PONG " + server->getServerName() + " " + command.getParam(0) + "\r\n";
	// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	sender->setOutBuffer(response);
}