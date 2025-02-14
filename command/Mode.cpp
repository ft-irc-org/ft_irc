#include "Mode.hpp"

Mode::Mode() {
}

Mode::~Mode() {
}

void Mode::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
    (void)clients;
    (void)server;

    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " MODE :Not enough parameters\r\n";
        sender->setOutBuffer(response);
        return;
    }

    std::string channelName = command.getParam(0);
    if (channelName[0] != '#') {
        return;
    }

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        std::string response = ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + " :No such channel\r\n";
        sender->setOutBuffer(response);
        return;
    }

    Channel* channel = channels[channelName];
    if (!auth.isOperator(sender->getNickname(), channelName) || !auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
        std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + " " + channelName + " :You're not channel operator :: MODE\r\n";
        sender->setOutBuffer(response);
        return;
    }

    if (command.getParamCount() == 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " MODE :Not enough parameters\r\n";
        sender->setOutBuffer(response);
        return;
    }

    std::string mode = command.getParam(1);
    if (mode[0] != '+' && mode[0] != '-') {
        std::string response = ":" + server->getServerName() + " 472 " + sender->getNickname() + " " + channelName + " :Unknown MODE flag\r\n";
        sender->setOutBuffer(response);
        return;
    }

    MODEOPERATION operation = (mode[0] == '+') ? MODE_ADD : MODE_REMOVE;
    mode = mode.substr(1);
    
    switch (mode[0]) {
        case CAHNNELMODE_INVITE_ONLY:
            modifyInviteOnlyMode(channel, operation);
            break;
        case CAHNNELMODE_TOPIC_RESTRICTED:
            modifyTopicRestrictedMode(channel, operation);
            break;
        case CAHNNELMODE_KEY:
            if (command.getParamCount() < 3) {
                std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " MODE :Not enough parameters\r\n";
                sender->setOutBuffer(response);
                return;
            }
            modifyKeyMode(channel, operation, command.getParam(2));
            break;
        case CAHNNELMODE_OPERATOR:
            if (command.getParamCount() < 3) {
                std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " MODE :Not enough parameters\r\n";
                sender->setOutBuffer(response);
                return;
            }
            modifyOperatorMode(auth, channel, operation, command.getParam(2));
            break;
        case CAHNNELMODE_LIMIT:
            if (command.getParamCount() < 3) {
                std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " MODE :Not enough parameters\r\n";
                sender->setOutBuffer(response);
                return;
            }
            modifyLimitMode(channel, operation, command.getParam(2));
            break;
        default:
            std::string response = ":" + server->getServerName() + " 472 " + sender->getNickname() + " " + channelName + " :Unknown MODE flag\r\n";
            sender->setOutBuffer(response);
            return;
    }
    std::string response = ":" + server->getServerName() + " 324 " + sender->getNickname() + " " + channelName + " " + channel->getModeString() + "\r\n";
    sender->setOutBuffer(response);
    channel->broadcast(response, sender, server);
}

void Mode::modifyInviteOnlyMode(Channel* channel, MODEOPERATION operation){
    if (operation == MODE_ADD) {
        channel->setChannelMode(Channel::INVITE_ONLY);
        channel->addCurrentUsersToWhiteList();
    } else {
        channel->unsetChannelMode(Channel::INVITE_ONLY);
        channel->removeAllUsersInWhiteList();
    }
}

void Mode::modifyTopicRestrictedMode(Channel* channel, MODEOPERATION operation){
    if (operation == MODE_ADD) {
        channel->setChannelMode(Channel::TOPIC_RESTRICTED);
    } else {
        channel->unsetChannelMode(Channel::TOPIC_RESTRICTED);
    }

}
void Mode::modifyKeyMode(Channel* channel, MODEOPERATION operation, const std::string& param){
    if (operation == MODE_ADD) {
        channel->setChannelMode(Channel::KEY_REQUIRED);
        channel->setPassword(param);
    } else {
        channel->unsetChannelMode(Channel::KEY_REQUIRED);
        channel->setPassword("");
    }

}
void Mode::modifyOperatorMode(Auth& auth, Channel* channel, MODEOPERATION operation, const std::string& param){
    if (operation == MODE_ADD) {
        auth.grantOperator(param, channel->getChannelName());
    } else {
        auth.revokeOperator(param, channel->getChannelName());
    }

}
void Mode::modifyLimitMode(Channel* channel, MODEOPERATION operation, const std::string& param){
    if (operation == MODE_ADD) {
        channel->setChannelMode(Channel::USER_LIMIT);
        std::stringstream ss(param);
        unsigned int userLimit;
        ss >> userLimit;
        channel->setUserLimit(userLimit);
    } else {
        channel->unsetChannelMode(Channel::USER_LIMIT);
        channel->setUserLimit(0);
    }
}