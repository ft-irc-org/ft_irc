#ifndef JOIN_HPP
# define JOIN_HPP

#include "CommandHandler.hpp"

class Join : public CommandHandler {
	public:
		Join();
		~Join();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		bool verifyChannelSyntax(Client* sender, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif