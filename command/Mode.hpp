#ifndef MODE_HPP
# define MODE_HPP

#include <sstream>
#include "CommandHandler.hpp"

class Mode : public CommandHandler {
	public:
		Mode();
		~Mode();
		void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth);

	private:
	void handleChannelMode(Client* sender, const std::string& channelName, const std::string& modes, const std::string& param, std::map<std::string, Channel*>& channels, Auth& auth);

	void handleModeQuery(Client* sender, const std::string& target, std::map<std::string, Channel*>& channels);

	std::string getModeString(Channel* channel);
};

#endif