#include "Channel.hpp"

Channel::Channel() : channelFd(-1), channelMode(0), userCount(0) {
}

Channel::Channel(int channelFd, const std::string& channelName) : channelFd(channelFd), channelName(channelName), channelMode(0), userCount(0) {
}

Channel::Channel(int channelFd, const std::string& channelName, unsigned int channelMode) : channelFd(channelFd), channelName(channelName), channelMode(channelMode), userCount(0) {
}

Channel::Channel(int channelFd, const std::string& channelName, const std::string& topic) : channelFd(channelFd), channelName(channelName), topic(topic), channelMode(0), userCount(0) {
}

Channel::Channel(int channelFd, const std::string& channelName, const std::string& topic, unsigned int channelMode, unsigned int userCount) : channelFd(channelFd), channelName(channelName), topic(topic), channelMode(channelMode), userCount(userCount) {
}

Channel::~Channel() {
}

int Channel::getChannelFd() const { return channelFd; }
const std::string& Channel::getChannelName() const { return channelName; }
const std::string& Channel::getTopic() const { return topic; }
unsigned int Channel::getChannelMode() const { return channelMode; }
unsigned int Channel::getUserCount() const { return userCount; }

void Channel::setChannelFd(int channelFd) { this->channelFd = channelFd; }
void Channel::setChannelName(const std::string& channelName) { this->channelName = channelName; }
void Channel::setTopic(const std::string& topic) { this->topic = topic; }
void Channel::setChannelMode(unsigned int channelMode) { this->channelMode = channelMode; }
void Channel::setUserCount(unsigned int userCount) { this->userCount = userCount; }

