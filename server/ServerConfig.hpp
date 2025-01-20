#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <string>

class ServerConfig {
	public:
		ServerConfig();
		ServerConfig(int port, const std::string& pw);
		~ServerConfig();

		ServerConfig &operator=(const ServerConfig &rhs);
        
		int getPort() const;
		const std::string& getPassword() const;

	private:
		const int port;
		const std::string password;
};

#endif