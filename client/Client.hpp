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

		void setAuthentication(bool status);
		void setBuffer(const std::string& buf);
		void setOutBuffer(const std::string& buf);
		void setNickname(const std::string& username);
		void setRealname(const std::string& realname);
		void setPassAuthenticated(bool status);
		void setNickAuthenticated(bool status);
		void setUserAuthenticated(bool status);

		int getSocketFd() const;
		const std::string& getIp() const;
		int getPort() const;
		bool isAuthenticated() const;
		bool isPassAuthenticated() const;
		bool isNickAuthenticated() const;
		bool isUserAuthenticated() const;
		std::string &getBuffer();
		std::string &getOutBuffer();
		const std::string& getNickname() const;
		const std::string& getRealname() const;

		void setCurrentChannel(const std::string& channel);
		void removeCurrentChannel();
		void updateLastPingTime();
    	time_t getLastPingTime() const;
    	void setAwaitingPong(bool value);
    	bool isAwaitingPong() const;
		void setLastPingTime(time_t time);


	private:
		int socketFd;
		std::string ip;
		int port;
		std::string nickname;
		std::string realname;
		bool authenticated;
		bool nickAuthenticated;
		bool passAuthenticated;
		bool userAuthenticated;
		std::string current_channel;

		std::string buffer;
		std::string outBuffer;

		time_t lastPingTime;
    	bool awaitingPong;
};

#endif