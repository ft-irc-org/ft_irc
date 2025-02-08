#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include "../client/Client.hpp"
#include "../message/Message.hpp"
#include "../channel/Channel.hpp"
#include "../client/Auth.hpp"
#include "../server/ServerEventHandler.hpp"

class CommandHandler {
	public:
		virtual ~CommandHandler() {}
		virtual void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) = 0;
		void sendError(Client* client, const std::string& error) {
			std::string response = error;
			// send(client->getSocketFd(), response.c_str(), response.size(), 0);
			client->setOutBuffer(response);
		}
		void sendMessage(Client* client, const std::string& message) {
			std::string response = message;
			// send(client->getSocketFd(), response.c_str(), response.size(), 0);
			client->setOutBuffer(response);
		}
		void disconnectClient(Client* client, ServerEventHandler* server, std::map<int, Client*> &clients) {
            if (client && server) {
                int fd = client->getSocketFd();
                server->removeReadEvent(fd);
                server->removeWriteEvent(fd);
                clients.erase(fd);
                close(fd);
            }
        }

		bool isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorCode, const std::string& errorMessage) {
			if (command.getParamCount() < minRequiredParams) {
				sendError(sender,  ":" + server->getServerName() + " " + errorCode + " " + sender->getNickname() + errorMessage);
				return false;
			}
			return true;
		}
		
		bool verifyChannelSyntax(Client* sender, ServerEventHandler *server, const std::string& channelName, const std::string& errorCode, const std::string& errorMessage) {
			if (channelName[0] != '#') {
				sendError(sender, ":" + server->getServerName() + " " + errorCode + " " + sender->getNickname() + " " + channelName + errorMessage);
				return false;
			}
			return true;
		}

		bool validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorCode, const std::string& errorMessage) {
			std::map<std::string, Channel*>::iterator it = channels.find(channelName);
			if (it == channels.end()) {
				sendError(sender, ":" + server->getServerName() + " " + errorCode + " " + sender->getNickname() + " " + channelName + errorMessage);
				return false;
			}
			return true;
		}
};

#endif