#include "Ping.hpp"

Ping::Ping() {
}

Ping::~Ping() {
}

void Ping::execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels) {
	std::string response = ":localhost PONG localhost\r\n";
	send(client->getSocketFd(), response.c_str(), response.size(), 0);
}