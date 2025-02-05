#ifndef PASS_HPP
# define PASS_HPP

#include "CommandHandler.hpp"

class Pass : public CommandHandler {
	public:
		Pass();
		~Pass();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif