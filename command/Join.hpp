#ifndef JOIN_HPP
# define JOIN_HPP

#include "CommandHandler.hpp"

class Join : public CommandHandler {
	public:
		Join();
		~Join();

		// 이 명령어에만 사용이 가능한 함수들
		Channel* createChannel(Client* sender, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server, std::string& channelName);
		bool checkInviteOnlyAccess(Client* sender, Auth &auth, ServerEventHandler *server, const std::string& channelName);
		bool checkChannelPassword(Client* sender, ServerEventHandler *server, Channel* channel, const std::string& password, const std::string& channelName);
		bool checkUserLimit(Client* sender, ServerEventHandler *server, Channel* channel, const std::string& channelName);

		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif