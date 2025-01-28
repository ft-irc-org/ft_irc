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
#include "ServerEventHandler.hpp"
#include "../channel/Channel.hpp"
#include "../command/Dispatcher.hpp"

# define MAX_CLIENTS 10

class Dispatcher;

class Server : public ServerEventHandler {
	public:
        Server(const ServerConfig& config);
        ~Server();

        void start();

        void removeReadEvent(int clientFd);
        void removeWriteEvent(int clientFd);

        std::map<std::string, Channel*>& getChannels();
        std::map<int, Client*>& getClients();

    private:
        ServerConfig config;
        std::map<std::string, Channel*> channels; // channelName, channel
        std::map<int, Client*> clients; // fd, client
        Dispatcher* dispatcher; // 포인터로 변경

        uintptr_t serverSocketFd;
        int kqueueFd;
        struct kevent events[MAX_CLIENTS];

        void acceptClient();
        void handleClientRead(int clientFd);
        void handleClientWrite(int clientFd);

        void addWriteEvent(int clientFd);
        void addReadEvent(int clientFd);

        int initSocket();
        int initKqueue();
};

#endif
