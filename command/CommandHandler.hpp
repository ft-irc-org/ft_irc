#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>
#include <string>
#include <map>
#include "../client/Client.hpp"
#include "../message/Message.hpp"
#include "../channel/Channel.hpp"
#include "../client/Auth.hpp"

class CommandHandler {
	public:
		virtual ~CommandHandler() {};
		virtual void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) = 0;
		void sendError(Client* client, const std::string& error) {
			std::string response = ":localhost " + error + "\r\n";
			send(client->getSocketFd(), response.c_str(), response.size(), 0);
		}
};

#endif