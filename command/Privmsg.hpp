#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "CommandHandler.hpp"

class Privmsg : public CommandHandler {
	public:
		Privmsg();
		~Privmsg();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif