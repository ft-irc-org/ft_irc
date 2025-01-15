#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string& ip, int port, const std::string& pw) : ip(ip), port(port), password(pw) {}
ServerConfig::~ServerConfig() {}

int ServerConfig::getPort() const {
	return port;
}

const std::string& ServerConfig::getPassword() const {
	return password;
}

const std::string& ServerConfig::getIp() const {
	return ip;
}