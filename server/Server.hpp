#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <exception>
#include <map>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/event.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "ServerConfig.hpp"
#include "../client/Client.hpp"
// #include "../client/Auth.hpp"
#include "../channel/Channel.hpp"
#include "../command/Dispatcher.hpp"

# define MAX_CLIENTS 10

class Server {
	public:
		Server(const ServerConfig& config);
		~Server();

		void start();

	private:
		ServerConfig config;
		Dispatcher dispatcher;
		std::map<std::string, Channel*> channels; // channelName, channel
		std::map<int, Client*> clients; // fd, client

		
		// std::map<int, Client*> clients; // fd, client
		// std::map<int, Channel*> channels; // fd, channel

		uintptr_t	serverSocketFd;
		int kqueueFd;
		struct kevent events[MAX_CLIENTS];

		void acceptClient();
		void handleClientEvent(int clientFd);

		int initSocket();
		int initKqueue();
};

#endif
