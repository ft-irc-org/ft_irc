#ifndef PONG_HPP
#define PONG_HPP

#include "CommandHandler.hpp"

class Pong : public CommandHandler {
	public:
		Pong();
		~Pong();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif