#include "Kick.hpp"

Kick::Kick() {
}

Kick::~Kick() {
}

bool Kick::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

bool Kick::verifyChannelSyntax(Client* sender, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
	if (channelName[0] != '#') {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
		return false;
	}
	return true;
}

bool Kick::validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
		return;
	}
}

void Kick::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
	(void)clients;
	(void)server;

	if (isParamCountValid(sender, command, server, 2, " KICK :Not enough parameters\r\n") == false) {
		return;
	}

	std::string channelName = command.getParam(0);
	std::string targetNickname = command.getParam(1);

	if (verifyChannelSyntax(sender, server, channelName, " :No such channel\r\n") == false) {
		return ;
	}

	if (validateChannelExists(sender, channels, server, channelName, " :No such channel\r\n") == false) {
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