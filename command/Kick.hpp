#ifndef KICK_HPP
# define KICK_HPP

#include "CommandHandler.hpp"

class Kick : public CommandHandler {
	public:
		Kick();
		~Kick();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif