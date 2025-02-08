#include "Ping.hpp"

Ping::Ping() {
}

Ping::~Ping() {
}

void Ping::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
	(void) clients;
	(void) channels;
	(void) auth;
	if (isParamCountValid(sender, command, server, 1, "461", " PING :Not enough parameters\r\n") == false) {
		return;
	}
}