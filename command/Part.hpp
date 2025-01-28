#ifndef PART_HPP
#define PART_HPP

#include "CommandHandler.hpp"

class Part : public CommandHandler {
	public:
		Part();
		~Part();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif