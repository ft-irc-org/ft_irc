#include "Client.hpp"

Client::Client() : socketFd(-1), authenticated(false), passAuthenticated(false) {
}

Client::Client(int socketFd, const std::string& ip, int port) : socketFd(socketFd), ip(ip), port(port), authenticated(false), passAuthenticated(false) {
}

Client::~Client() {
}

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
