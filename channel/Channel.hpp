#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <exception>
# include <map>
# include "../client/Client.hpp"
# include "../client/Auth.hpp"

class Channel {
	public:
		Channel(int channelFd, const std::string& channelName, const std::string& topic, unsigned int channelMode, unsigned int userCount);
		Channel(int channelFd, const std::string& channelName, const std::string& topic);
		Channel(int channelFd, const std::string& channelName, unsigned int channelMode);
		Channel(int channelFd, const std::string& channelName);
		Channel();
		~Channel();

		int getChannelFd() const;
		const std::string& getChannelName() const;
		const std::string& getTopic() const;
		unsigned int getChannelMode() const;
		unsigned int getUserCount() const;

		void setChannelFd(int channelFd);
		void setChannelName(const std::string& channelName);
		void setTopic(const std::string& topic);
		void setChannelMode(unsigned int channelMode);
		void setUserCount(unsigned int userCount);

		void addMember(const Client& client);
		void removeMember(const Client& client);

		void broadcast(const std::string& message, const Client& sender);

	private:
		int channelFd;
		std::string channelName;
		std::string topic;
		unsigned int channelMode;

		std::map<int, Client> users;
		std::map<int, Channel> channels;
};

#endif