#ifndef JOIN_HPP
# define JOIN_HPP

#include "CommandHandler.hpp"

class Join : public CommandHandler {
	public:
		Join();
		~Join();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif