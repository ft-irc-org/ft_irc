#ifndef COMMANDDISPATCHER_HPP
# define COMMANDDISPATCHER_HPP

# include <iostream>
# include <string>
# include <map>
# include "CommandHandler.hpp"
# include "../client/Client.hpp"
# include "../client/Auth.hpp"
# include "../message/Message.hpp"
# include "../channel/Channel.hpp"
# include "../server/ServerConfig.hpp"

# include "Join.hpp"
# include "Kick.hpp"
# include "Privmsg.hpp"
# include "Nick.hpp"
# include "Ping.hpp"
# include "Pong.hpp"
# include "Mode.hpp"
# include "Quit.hpp"
# include "Pass.hpp"
# include "User.hpp"
# include "Whois.hpp"

class Dispatcher {
	public:
		Dispatcher(std::map<std::string, Channel*>& channels, std::map<int, Client*>& clients, 
                  const ServerConfig& config);

		~Dispatcher();

		void registerHandler(const std::string& command, CommandHandler* handler);
		void dispatch(Client* client, const Message& command);

		// Add methods to modify server state
		void addClient(int fd, Client* client);
		void removeClient(int fd);
		void addChannel(const std::string& name, Channel* channel);
		void removeChannel(const std::string& name);

	private:
		Auth auth;
		std::map<std::string, CommandHandler*> handlers;
		std::map<std::string, Channel*>& channels;
		std::map<int, Client*>& clients;

};

#endif