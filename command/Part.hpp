#ifndef PART_HPP
# define PART_HPP

#include "CommandHandler.hpp"

class Part : public CommandHandler {
	public:
		Part();
		~Part();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif