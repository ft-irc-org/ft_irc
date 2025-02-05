#ifndef PING_HPP
# define PING_HPP

#include "CommandHandler.hpp"

class Ping : public CommandHandler {
	public:
		Ping();
		~Ping();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif