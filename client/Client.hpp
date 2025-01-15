#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
	public:
		Client(int socketFd, const std::string& ip);
		~Client();

	private:
		int socketFd;
		std::string ip;
		std::string nickname;
		std::string username;
		bool connected;
		std::string buffer;
};

#endif