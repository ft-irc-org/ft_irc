#ifndef PONG_HPP
#define PONG_HPP

#include "CommandHandler.hpp"

class Pong : public CommandHandler {
	public:
		Pong();
		~Pong();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif