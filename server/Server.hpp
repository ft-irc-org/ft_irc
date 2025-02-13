#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
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
#include "ServerEventHandler.hpp"
#include "../channel/Channel.hpp"
#include "../command/Dispatcher.hpp"

# define MAX_CLIENTS 10

class Dispatcher;

class Server : public ServerEventHandler {
	public:
        Server(const ServerConfig& config, const std::string& serverName);
        ~Server();

        void start();

        void addWriteEvent(int clientFd);
        void removeReadEvent(int clientFd);
        void removeWriteEvent(int clientFd);
        const std::string &getServerName();

    private:
        ServerConfig config;
        std::map<std::string, Channel*> channels; // channelName, channel
        std::map<int, Client*> clients; // fd, client
        Dispatcher* dispatcher; // 포인터로 변경

        std::string serverName;
        uintptr_t serverSocketFd;
        int kqueueFd;
        struct kevent events[MAX_CLIENTS];

        void acceptClient();
        void handleClientRead(int clientFd);
        void handleClientWrite(int clientFd);

        void addReadEvent(int clientFd);
        void checkTimeout();

        int initSocket();
        int initKqueue();
};

#endif
