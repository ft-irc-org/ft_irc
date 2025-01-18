#ifndef PING_HPP
# define PING_HPP

#include "CommandHandler.hpp"

class Ping : public CommandHandler {
	public:
		Ping();
		~Ping();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif