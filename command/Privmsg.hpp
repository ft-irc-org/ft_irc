#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "CommandHandler.hpp"

class Privmsg : public CommandHandler {
	public:
		Privmsg();
		~Privmsg();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif