#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include <exception>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>

class Client {
	public:
		Client(int socketFd, const std::string& ip, int port);
		Client();
		~Client();

		int getSocketFd() const;
		const std::string& getIp() const;
		int getPort() const;
		bool isConnected() const;
		const std::string& getBuffer() const;

	private:
		int socketFd;
		std::string ip;
		int port;
		std::string nickname;
		std::string username;
		bool connected;
		std::string buffer;

		void setBuffer(const std::string& buf);
		void setConnected(bool status);
};

#endif