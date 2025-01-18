#include "Quit.hpp"

Quit::Quit() {
}

Quit::~Quit() {
}

void Quit::execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels) {
	std::string response = ":" + client->getNickname() + " QUIT\r\n";
	send(client->getSocketFd(), response.c_str(), response.size(), 0);
	client->setConnected(false);
}