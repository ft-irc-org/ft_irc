#ifndef COMMANDDISPATCHER_HPP
# define COMMANDDISPATCHER_HPP

# include <iostream>
# include <string>
# include <map>
# include "CommandHandler.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"

class CommandDispatcher {
	public:
		CommandDispatcher();
		~CommandDispatcher();

		void registHandler(const std::string& command, CommandHandler* handler);
		void dispatch(Client* client, const std::string& command);

	private:
		//unused
		std::map<std::string, CommandHandler*> handlers;
		std::map<std::string, Channel*>& channels;
		std::map<int, Client*>& clients;
};

#endif