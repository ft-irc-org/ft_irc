#ifndef PASS_HPP
# define PASS_HPP

#include "CommandHandler.hpp"

class Pass : public CommandHandler {
	public:
		Pass();
		~Pass();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif