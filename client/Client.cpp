#include "Client.hpp"

Client::Client() : socketFd(-1), authenticated(false), passAuthenticated(false) {
}

Client::Client(int socketFd, const std::string& ip, int port) : socketFd(socketFd), ip(ip), port(port), authenticated(false), passAuthenticated(false) {
	awaitingPong = false;
	lastPingTime = time(NULL);
}

Client::~Client() {
}

void Client::updateLastPingTime() { lastPingTime = time(NULL); }
time_t Client::getLastPingTime() const { return lastPingTime; }
void Client::setAwaitingPong(bool value) { awaitingPong = value; }
bool Client::isAwaitingPong() const { return awaitingPong; }
// getter
int Client::getSocketFd() const { return socketFd; }
const std::string& Client::getIp() const { return ip; }
int Client::getPort() const { return port; }
std::string& Client::getBuffer() { return buffer; }
bool Client::isAuthenticated() const { return authenticated; }
const std::string& Client::getNickname() const { return nickname; }
const std::string& Client::getRealname() const { return realname; }
bool Client::isPassAuthenticated() const { return passAuthenticated; }
bool Client::isNickAuthenticated() const { return nickAuthenticated; }
bool Client::isUserAuthenticated() const { return userAuthenticated; }
std::string& Client::getOutBuffer() { return outBuffer; }
// setter
void Client::setAuthentication(bool status) { authenticated = status; }
void Client::setPassAuthenticated(bool status) { passAuthenticated = status; }
void Client::setNickname(const std::string& nickname) { this->nickname = nickname; }
void Client::setNickAuthenticated(bool status) { nickAuthenticated = status; }
void Client::setUserAuthenticated(bool status) { userAuthenticated = status; }
void Client::setRealname(const std::string& realname) { this->realname = realname; }
void Client::setBuffer(const std::string& buf) {
	buffer += buf;
}
void Client::setOutBuffer(const std::string& buf) {
	outBuffer += buf;
}
void Client::setCurrentChannel(const std::string& channel) {
	current_channel = channel;
}
void Client::removeCurrentChannel() {
	current_channel = "";
}
void Client::setLastPingTime(time_t time) {
	lastPingTime = time;
}

