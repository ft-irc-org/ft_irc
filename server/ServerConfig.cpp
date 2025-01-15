#include "ServerConfig.hpp"

ServerConfig::ServerConfig(int port, const std::string& pw) : port(port), password(pw) {}
ServerConfig::~ServerConfig() {}

int ServerConfig::getPort() const {
	return port;
}

const std::string& ServerConfig::getPassword() const {
	return password;
}

