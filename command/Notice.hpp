#ifndef NOTICE_HPP
# define NOTICE_HPP

#include "CommandHandler.hpp"

class Notice : public CommandHandler {
	public:
		Notice();
		~Notice();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif