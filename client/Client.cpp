#include "Client.hpp"

Client::Client() : socketFd(-1), connected(false) {
}

Client::Client(int socketFd, const std::string& ip, int port) : socketFd(socketFd), ip(ip), port(port), connected(true) {
}

Client::~Client() {
}

// getter
int Client::getSocketFd() const { return socketFd; }
const std::string& Client::getIp() const { return ip; }
int Client::getPort() const { return port; }
bool Client::isConnected() const { return connected; }
const std::string& Client::getBuffer() const { return buffer; }

// setter
void Client::setBuffer(const std::string& buf) { buffer = buf; }
void Client::setConnected(bool status) { connected = status; }