#ifndef PING_HPP
# define PING_HPP

#include "CommandHandler.hpp"

class Ping : public CommandHandler {
	public:
		Ping();
		~Ping();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif