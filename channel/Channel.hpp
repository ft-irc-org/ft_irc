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
		typedef enum ChannelMode {
			INVITE_ONLY = 1 << 0,
			TOPIC_RESTRICTED = 1 << 1,
			KEY_REQUIRED = 1 << 2,
			USER_LIMIT = 1 << 4,
			OPERATOR_PRIVILEGES = 1 << 5
		} ChannelMode;

		Channel(int channelFd, const std::string& channelName);
		Channel();
		~Channel();

		int getChannelFd() const;
		const std::string& getChannelName() const;
		const std::string& getTopic() const;
		const std::string& getTopicSetter() const;
    	time_t getTopicTime() const;
    
		const std::string& getPassword() const;
		unsigned int getChannelMode() const;
		unsigned int getUserCount() const;
		unsigned int getUserLimit() const;
		std::map<int, Client *> getUsers() const;

		void setChannelFd(int channelFd);
		void setChannelName(const std::string& channelName);
    	void setTopic(const std::string& newTopic, const std::string& setter);
		void setPassword(const std::string& password);
		void setUserLimit(unsigned int userLimit);

		bool hasMode(unsigned int requestMode) const;
		bool isMember(Client *client) const;
		bool isOperator(Client *client) const;

		void addMember(Client *client);
		void removeMember(Client *client);
		Client* searchMember(const std::string targeName) const;
		void broadcast(const std::string& message, Client* sender);

		void setChannelMode(unsigned int channelMode);
		void unsetChannelMode(unsigned int requestMode);
		void setServerName(const std::string& serverName);
		std::string getModeString() const;


	private:
		static int nextFd;
		int channelFd;
		std::string channelName;
		std::string topic;
	    std::string topicSetter;
    	time_t topicTime;
		unsigned int channelMode;
		unsigned int userCount;
		unsigned int userLimit;
		std::string password;
		std::string serverName;

		std::map<int, Client *> users;
};

#endif