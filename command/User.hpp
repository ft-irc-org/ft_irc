#ifndef USER_HPP
# define USER_HPP

#include "CommandHandler.hpp"

class User : public CommandHandler {
	public:
		User();
		~User();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif