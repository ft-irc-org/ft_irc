#include "Quit.hpp"

Quit::Quit() {
}

Quit::~Quit() {
}

void Quit::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	(void) command;
	std::string response = ":" + sender->getNickname() + " QUIT\r\n";
	// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
	sender->setOutBuffer(response);
	sender->setAuthentication(false);
	close(sender->getSocketFd());
	clients.erase(sender->getSocketFd());
	auth.removeUser(sender->getNickname());
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		it->second->removeMember(sender);
	}
}