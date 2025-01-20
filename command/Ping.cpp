#include "Ping.hpp"

Ping::Ping() {
}

Ping::~Ping() {
}

void Ping::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth){
	if (command.getParamCount() < 1) {
		std::string response = ":localhost 461 " + sender->getNickname() + " PING :Not enough parameters\r\n";
		send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		return;
	}

	std::string response = ":localhost PONG localhost " + command.getParam(0) + "\r\n";
	send(sender->getSocketFd(), response.c_str(), response.size(), 0);
}