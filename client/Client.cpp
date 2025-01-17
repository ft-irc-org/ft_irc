#include "Client.hpp"

Client::Client() : socketFd(-1), connected(false) {
}

Client::Client(int socketFd, const std::string& ip, int port) : socketFd(socketFd), ip(ip), port(port), authenticated(false), connected(true) {
}

Client::~Client() {
}

// getter
int Client::getSocketFd() const { return socketFd; }
const std::string& Client::getIp() const { return ip; }
int Client::getPort() const { return port; }
bool Client::isConnected() const { return connected; }
const std::string& Client::getBuffer() const { return buffer; }
bool Client::isAuthenticated() const { return authenticated; }
const std::string& Client::getNickname() const { return nickname; }

// setter
void Client::setAuthentication(bool status) { authenticated = status; }
void Client::setNickname(const std::string& nickname) { this->nickname = nickname; }
void Client::setConnected(bool status) { connected = status; }

// unused setter
void Client::setBuffer(const std::string& buf) { buffer = buf; }
