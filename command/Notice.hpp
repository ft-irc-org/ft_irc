#ifndef NOTICE_HPP
# define NOTICE_HPP

#include "CommandHandler.hpp"

class Notice {
	public:
		Notice();
		~Notice();
		void execute(Client* client, const Message& command, std::map<std::string, Channel*>& channels);
};

#endif