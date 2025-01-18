#ifndef QUIT_HPP
# define QUIT_HPP

#include "CommandHandler.hpp"

class Quit : public CommandHandler {
	public:
		Quit();
		~Quit();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif