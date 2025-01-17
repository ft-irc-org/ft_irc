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

		void setConnected(bool status);
		void setAuthentication(bool status);
		void setNickname(const std::string& nickname);

		int getSocketFd() const;
		const std::string& getIp() const;
		int getPort() const;
		bool isConnected() const;
		bool isAuthenticated() const;
		const std::string& getNickname() const;

		const std::string& getBuffer() const;

	private:
		int socketFd;
		std::string ip;
		int port;
		std::string nickname;
		std::string username;
		bool authenticated;
		bool connected;

		//unused
		std::string buffer;

		void setBuffer(const std::string& buf);
};

#endif