#ifndef KICK_HPP
# define KICK_HPP

#include "CommandHandler.hpp"

class Kick : public CommandHandler {
	public:
		Kick();
		~Kick();
		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage);
		bool verifyChannelSyntax(Client* sender, ServerEventHandler *server, const std::string& channelName, const std::string& errorMessage);
		bool validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage);
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
	/**
	 * Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERNOTINCHANNEL (441)
ERR_NOTONCHANNEL (442)
Deprecated Numeric Reply:

ERR_BADCHANMASK (476)
	*/
	private:
		typedef enum {
			ERR_NEEDMOREPARAMS = 461,
			ERR_NOSUCHCHANNEL = 403,
			ERR_CHANOPRIVSNEEDED = 482,
			ERR_USERNOTINCHANNEL = 441,
			ERR_NOTONCHANNEL = 442,
			ERR_BADCHANMASK = 476
		} NumericReply;

};

#endif