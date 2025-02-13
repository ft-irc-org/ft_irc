#include "Channel.hpp"

int Channel::nextFd = 0;

Channel::Channel() : channelName(""), topic(""), topicSetter(""), topicTime(0), channelMode(0), userCount(0), userLimit(0), password("")  {
	channelFd = nextFd++;
}

Channel::Channel(int fd, const std::string& channelName) : channelName(channelName), topic(""), topicSetter(""), topicTime(0), channelMode(0), userCount(0), userLimit(0), password("") {
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
const std::string& Channel::getTopicSetter() const { return topicSetter; }
time_t Channel::getTopicTime() const { return topicTime; }
unsigned int Channel::getChannelMode() const { return channelMode; }
unsigned int Channel::getUserLimit() const { return userLimit; }
unsigned int Channel::getUserCount() const {
	return users.size();
 }
std::map<int, Client *> Channel::getUsers() const { return users; }

bool Channel::isMember(Client *client) const {
	return users.find(client->getSocketFd()) != users.end();
}
bool Channel::isOperator(Client *client) const {
	return hasMode(OPERATOR_PRIVILEGES) && isMember(client);
}
void Channel::setChannelFd(int channelFd) { this->channelFd = channelFd; }
void Channel::setChannelName(const std::string& channelName) { this->channelName = channelName; }
void Channel::setTopic(const std::string& newTopic, const std::string& setter) {         
	this->topic = newTopic;
    this->topicSetter = setter;
    this->topicTime = time(NULL);; 
}
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

void Channel::addMember(Client* client){
    users[client->getSocketFd()] = client;
    userCount++;
}

void Channel::removeMember(Client* client){
	users.erase(client->getSocketFd());
}

Client* Channel::searchMember(const std::string targeName) const {
	for (std::map<int, Client*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second->getNickname() == targeName) {  // 포인터이므로 ->로 접근
			return it->second;  // 포인터 반환
		}
	}
	return NULL;
}

void Channel::broadcast(const std::string& message, Client* sender, const std::string& command, ServerEventHandler *server) {
	if (!isMember(sender)) {
		std::string err = ":" + serverName + " 442 " + sender->getNickname() + " " + channelName + " :You're not on that channel\r\n";
		sender->setOutBuffer(err);
		return;
	}
	// if (sender == nullptr) {
	// 	for (std::map<int, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
	// 		std::string response = ":" + sender->getNickname() + " " + command + " " + channelName + " :" + message + "\r\n";
	// 		it->second->setOutBuffer(response);
	// 	}
	// 	return;
	// }
	// exit? quit? 전파 왜?
	for (std::map<int, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->first != sender->getSocketFd()) {
			std::string response = ":" + sender->getNickname() + " " + command + " " + channelName + " :" + message + "\r\n";
			server->addWriteEvent(it->first);
			it->second->setOutBuffer(response);
		}
	}
}

std::string Channel::getModeString() const {
	std::string mode = "";
	if (hasMode(INVITE_ONLY)) {
		mode += "i";
	}
	if (hasMode(TOPIC_RESTRICTED)) {
		mode += "t";
	}
	if (hasMode(KEY_REQUIRED)) {
		mode += "k";
	}
	if (hasMode(OPERATOR_PRIVILEGES)) {
		mode += "o";
	}
	if (hasMode(USER_LIMIT)) {
		mode += "l";
	}
	return mode;
}

void Channel::addWhiteList(Client *client) {
	whiteList[client->getSocketFd()] = client;
}

void Channel::removeWhiteList(Client *client) {
	whiteList.erase(client->getSocketFd());
}

void Channel::removeAllUsersInWhiteList() {
	whiteList.clear();
}

void Channel::addCurrentUsersToWhiteList() {
	for (std::map<int, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
		whiteList[it->first] = it->second;
	}
}

bool Channel::isWhiteList(Client *client) const {
	return whiteList.find(client->getSocketFd()) != whiteList.end();
}

bool Channel::hasMode(unsigned int requestMode) const {
	return (channelMode & requestMode) != 0;
}

void Channel::setServerName(const std::string& serverName) { this->serverName = serverName; }

void Channel::setPassword(const std::string& password) { this->password = password; }
const std::string& Channel::getPassword() const { return password; }
