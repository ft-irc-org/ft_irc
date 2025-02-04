#ifndef TOPIC_HPP
# define TOPIC_HPP

#include <unistd.h>

# include "CommandHandler.hpp"

class Topic : public CommandHandler {
	public:
		Topic();
		~Topic();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif