#include "Kick.hpp"

Kick::Kick() {
}

Kick::~Kick() {
}

void Kick::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void)clients;
	(void)server;

	if (command.getParamCount() < 2) {
		std::string response = ":localhost 461 " + sender->getNickname() + " KICK :Not enough parameters\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	std::string channelName = command.getParam(0);
	std::string targetNickname = command.getParam(1);

	if (channelName[0] != '#') {
		std::string response = ":localhost 403 " + sender->getNickname() + " " + channelName + " :No such channel\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		std::string response = ":localhost 403 " + sender->getNickname() + " " + channelName + " :No such channel\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	Channel* channel = channels[channelName];
	if (!auth.isOperator(sender->getNickname(), channelName) || !auth.hasPermission(sender->getNickname(), channelName, Auth::KICK_POWER)) {
		std::string response = ":localhost 482 " + sender->getNickname() + " " + channelName + " :You're not channel operator :: KICK\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	Client* target = channel->searchMember(targetNickname);
	if (target == NULL) {
		std::string response = ":localhost 401 " + sender->getNickname() + " " + targetNickname + " :No such nick/channel\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	if (auth.isOperator(targetNickname, channelName)) {
		std::string response = ":localhost 482 " + sender->getNickname() + " " + channelName + " :Cannot kick channel operator\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}
	
	std::string response = ":" + sender->getNickname() + " KICK " + channelName + " " + targetNickname + " :Kicked by " + sender->getNickname() + "\r\n";
	channel->broadcast(response, sender);
	channel->removeMember(target);
}