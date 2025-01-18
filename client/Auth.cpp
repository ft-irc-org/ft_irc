#include "Auth.hpp"

// AuthPK operator
bool Auth::AuthPK::operator<(const AuthPK& pk) const {
	if (userNickname == pk.userNickname)
		return channelName < pk.channelName;
	return userNickname < pk.userNickname;
}

Auth::Auth() {}

Auth::~Auth() {}

bool Auth::hasPermission(const std::string& userNickname, const std::string& channelName, int permissions) const {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	std::map<AuthPK, unsigned int>::const_iterator it = authMap.find(pk);
	if (it == authMap.end())
		return false;
	return (it->second & permissions) != 0;
}

bool Auth::isOperator(const std::string& userNickname, const std::string& channelName) const {
	return hasPermission(userNickname, channelName, OP);
}

void Auth::grantPermission(const std::string& userNickname, const std::string& channelName, int permissions) {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	authMap[pk] |= permissions;
}

void Auth::revokePermission(const std::string& userNickname, const std::string& channelName, int permissions) {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	std::map<AuthPK, unsigned int>::iterator it = authMap.find(pk);
	if (it != authMap.end()) {
		it->second &= ~permissions;
		if (it->second == NONE)
			authMap.erase(it);
	}
}

void Auth::grantOperator(const std::string& userNickname, const std::string& channelName) {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	authMap[pk] |= (OP | CHANNEL_MODE | KICK_POWER | INVITE_POWER | TOPIC_POWER);
}

void Auth::revokeOperator(const std::string& userNickname, const std::string& channelName) {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	authMap.erase(pk);
}

void Auth::removeChannel(const std::string& channelName) {
	std::map<AuthPK, unsigned int>::iterator it = authMap.begin();
	while (it != authMap.end()) {
		if (it->first.channelName == channelName)
			authMap.erase(it++);
		else
			++it;
	}
}

void Auth::removeUser(const std::string& userNickname) {
	std::map<AuthPK, unsigned int>::iterator it = authMap.begin();
	while (it != authMap.end()) {
		if (it->first.userNickname == userNickname)
			authMap.erase(it++);
		else
			++it;
	}
}

void Auth::removeUserFromChannel(const std::string& userNickname, const std::string& channelName) {
	AuthPK pk;
	pk.userNickname = userNickname;
	pk.channelName = channelName;
		
	authMap.erase(pk);
}

void Auth::updateNickname(const std::string& oldNickname, const std::string& newNickname) {
	std::map<AuthPK, unsigned int>::iterator it = authMap.begin();
	while (it != authMap.end()) {
		if (it->first.userNickname == oldNickname) {
			AuthPK pk;
			pk.userNickname = newNickname;
			pk.channelName = it->first.channelName;
			authMap[pk] = it->second;
			authMap.erase(it++);
		} else {
			++it;
		}
	}
}