#include "Part.hpp"

Part::Part() {
}

Part::~Part() {
}

void Part::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) server;
    (void) auth;

    if (command.getParamCount() < 1) {
        sender->setOutBuffer("461 " + sender->getNickname() + " PART :Not enough parameters\r\n");
        return;
    }

    std::string channelsParam = command.getParam(0);
    std::string reason;
    if (command.getParamCount() > 1) {
        reason = command.getParam(1);
    }

    size_t start = 0;
    size_t end = 0;
    while ((end = channelsParam.find(',', start)) != std::string::npos || start < channelsParam.length()) {
        std::string channelName;
        if (end != std::string::npos) {
            channelName = channelsParam.substr(start, end - start);
            start = end + 1;
        } else {
            channelName = channelsParam.substr(start);
            start = channelsParam.length();
        }

        if (channelName[0] != '#') {
            sender->setOutBuffer("475 " + sender->getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n");
            continue;
        }

        std::map<std::string, Channel*>::iterator it = channels.find(channelName);
        if (it == channels.end()) {
            sender->setOutBuffer("442 " + sender->getNickname() + " " + channelName + " :You're not on that channel\r\n");
            continue;
        }

        Channel* channel = it->second;
        if (!channel->isMember(sender)) {
            sender->setOutBuffer("442 " + sender->getNickname() + " " + channelName + " :You're not on that channel\r\n");
            continue;
        }

        std::string source = sender->getNickname() + "!" + sender->getRealname() + "@" + server->getServerName();
        std::string partMessage = ":" + source + " PART " + channelName;
        if (!reason.empty()) {
            partMessage += " :" + reason;
        }
        partMessage += "\r\n";

        sender->setOutBuffer(partMessage);
        channel->broadcast(partMessage, sender, "PART", server);
        channel->removeMember(sender);

        if (channel->getUserCount() == 0) {
            channels.erase(channelName);
            delete channel;
        }
    }
}