#ifndef NICK_HPP
# define NICK_HPP

#include "CommandHandler.hpp"

class Nick : public CommandHandler {
	public:
		Nick();
		~Nick();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
		void broadcastToChannels(const std::string& message, Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server);
		bool isValidNickname(const std::string& nickname) const;
};

#endif