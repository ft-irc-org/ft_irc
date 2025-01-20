#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : port(0), password("") {}
ServerConfig::ServerConfig(int port, const std::string& pw) : port(port), password(pw) {}
ServerConfig::~ServerConfig() {}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs) {
	if (this == &rhs) {
		return *this;
	}
	return *this;
}

int ServerConfig::getPort() const {
	return port;
}

const std::string& ServerConfig::getPassword() const {
	return password;
}

