#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <string>

class ServerConfig {
	public:
        ServerConfig(const std::string& ip, int port, const std::string& pw);
		~ServerConfig();
        
        int getPort() const;
		const std::string& getIp() const;
        const std::string& getPassword() const;

	private:
		const std::string ip;
		const int port;
		const std::string password;
};

#endif