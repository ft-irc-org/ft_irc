#include "Kick.hpp"

Kick::Kick() {
}

Kick::~Kick() {
}

void Kick::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void)clients;
	(void)server;

	if (isParamCountValid(sender, command, server, 2, "461", " KICK :Not enough parameters\r\n") == false) {
		return;
	}

	std::string channelName = command.getParam(0);
	std::string targetNickname = command.getParam(1);

	if (verifyChannelSyntax(sender, server, channelName, "403", " :No such channel\r\n") == false) {
		return ;
	}

	if (validateChannelExists(sender, channels, server, channelName, "403", " :No such channel\r\n") == false) {
		return ;
	}

	Channel* channel = channels[channelName];
	if (!auth.isOperator(sender->getNickname(), channelName) || !auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
		std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + " " + channelName + " :You're not channel operator :: KICK\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	Client* target = channel->searchMember(targetNickname);
	if (target == NULL) {
		std::string response = ":" + server->getServerName() + " 401 " + sender->getNickname() + " " + targetNickname + " :No such nick/channel\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}

	if (auth.isOperator(targetNickname, channelName)) {
		std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + " " + channelName + " :Cannot kick channel operator\r\n";
		// send(sender->getSocketFd(), response.c_str(), response.size(), 0);
		sender->setOutBuffer(response);
		return;
	}
	
	std::string response = ":" + sender->getNickname() + " KICK " + channelName + " " + targetNickname + " :Kicked by " + sender->getNickname() + "\r\n";
	channel->broadcast(response, sender);
	channel->removeMember(target);
}