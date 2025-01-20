#ifndef QUIT_HPP
# define QUIT_HPP

#include <sys/socket.h>
#include <unistd.h>

# include "CommandHandler.hpp"

class Quit : public CommandHandler {
	public:
		Quit();
		~Quit();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif