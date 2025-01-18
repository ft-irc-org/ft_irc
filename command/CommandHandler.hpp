#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>
#include <string>
#include <map>
#include "../client/Client.hpp"
#include "../channel/Channel.hpp"

class CommandHandler {
	public:
		virtual ~CommandHandler() = 0;
		virtual void execute(Client* client, const std::string& command, std::map<std::string, Channel*>& channels) = 0;
};

#endif