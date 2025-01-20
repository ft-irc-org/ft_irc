#ifndef WHOIS_HPP
# define WHOIS_HPP

#include "CommandHandler.hpp"

class Whois : public CommandHandler {
	public:
		Whois();
		~Whois();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);
};

#endif