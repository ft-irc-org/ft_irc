#include "Channel.hpp"

int Channel::nextFd = 0;

Channel::Channel() : channelName(""), topic(""), channelMode(0), userCount(0), userLimit(0), password("")  {
	channelFd = nextFd++;
}

Channel::Channel(int fd, const std::string& channelName) : channelName(channelName), topic(""), channelMode(0), userCount(0), userLimit(0), password("") {
	if (fd == -1) {
		channelFd = nextFd++;
    } else {
		channelFd = fd;
		if (fd >= nextFd) {
		nextFd = fd + 1;
		}
	}
}

Channel::~Channel() {
}

int Channel::getChannelFd() const { return channelFd; }
const std::string& Channel::getChannelName() const { return channelName; }
const std::string& Channel::getTopic() const { return topic; }
unsigned int Channel::getChannelMode() const { return channelMode; }
unsigned int Channel::getUserLimit() const { return userLimit; }
unsigned int Channel::getUserCount() const { return userCount; }

void Channel::setChannelFd(int channelFd) { this->channelFd = channelFd; }
void Channel::setChannelName(const std::string& channelName) { this->channelName = channelName; }
void Channel::setTopic(const std::string& topic) { this->topic = topic; }
void Channel::setUserLimit(unsigned int userLimit) { this->userLimit = userLimit; }
void Channel::setChannelMode(unsigned int requestMode) {
	if (requestMode == 0){
		channelMode = 0;
	} else {
		channelMode |= requestMode;
	}
}

void Channel::unsetChannelMode(unsigned int requestMode) {
	channelMode &= ~requestMode;
}

void Channel::setUserCount(unsigned int userCount) { this->userCount = userCount; }

void Channel::addMember(const Client& client){
	users[client.getSocketFd()] = client;
	userCount++;
}

void Channel::removeMember(const Client& client){
	users.erase(client.getSocketFd());
	userCount--;
}

Client *Channel::searchMember(const std::string targeName) const {
	for (std::map<int, Client>::const_iterator it = users.begin(); it != users.end(); ++it){
		if (it->second.getNickname() == targeName){
			return (Client *)&it->second;
		}
	}
	return NULL;
}

void Channel::broadcast(const std::string& message, const Client& sender){
	for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it){
		if (it->first != sender.getSocketFd()){
			std::string response = ":" + sender.getNickname() + " PRIVMSG " + channelName + " :" + message + "\r\n";
			send(it->first, response.c_str(), response.size(), 0);
		}
	}	
}

void Channel::setPassword(const std::string& password) { this->password = password; }
const std::string& Channel::getPassword() const { return password; }