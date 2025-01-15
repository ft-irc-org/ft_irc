#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <string>

class ServerConfig {
	public:
		ServerConfig(int port, const std::string& pw);
		~ServerConfig();
        
		int getPort() const;
		const std::string& getPassword() const;

	private:
		const int port;
		const std::string password;
};

#endif