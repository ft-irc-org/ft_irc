#ifndef MODE_HPP
# define MODE_HPP

#include <sstream>
#include "CommandHandler.hpp"

class Mode : public CommandHandler {
	public:
		Mode();
		~Mode();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);

	private:
		enum MODEOPERATION {
			MODE_ADD = '+',
			MODE_REMOVE = '-'
		};

		enum CHANNELFLAGS {
			CAHNNELMODE_INVITE_ONLY = 'i',
			CAHNNELMODE_TOPIC_RESTRICTED = 't',
			CAHNNELMODE_KEY = 'k',
			CAHNNELMODE_OPERATOR = 'o',
			CAHNNELMODE_LIMIT = 'l'
		};

		void modifyInviteOnlyMode(Channel* channel, MODEOPERATION operation);
		void modifyTopicRestrictedMode(Channel* channel, MODEOPERATION operation);
		void modifyKeyMode(Channel* channel, MODEOPERATION operation, const std::string& param);
		void modifyOperatorMode(Auth& auth, Channel* channel, MODEOPERATION operation, const std::string& param);
		void modifyLimitMode(Channel* channel, MODEOPERATION operation, const std::string& param);
};

#endif