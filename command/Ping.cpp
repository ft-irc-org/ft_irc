#include "Ping.hpp"

Ping::Ping() {
}

Ping::~Ping() {
}

bool Ping::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

void Ping::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
	(void) clients;
	(void) channels;
	(void) auth;
	if (isParamCountValid(sender, command, server, 1, " PING :Not enough parameters\r\n") == false) {
		return;
	}
}