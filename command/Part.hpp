#ifndef PART_HPP
#define PART_HPP

#include "CommandHandler.hpp"

class Part : public CommandHandler {
	public:
		Part();
		~Part();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		bool verifyChannelSyntax(Client* sender, ServerEventHandler *server, const std::string& channelName, const std::string& errorMessage);
		bool validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif