#include "Mode.hpp"

Mode::Mode() {
}

Mode::~Mode() {
}

bool Mode::isParamCountValid(Client* sender, const Message& command, ServerEventHandler *server, int minRequiredParams, const std::string& errorMessage) {
	if (command.getParamCount() < minRequiredParams) {
		sendError(sender,  ":" + server->getServerName() + " 461 " + sender->getNickname() + errorMessage);
        return false;
    }
	return true;
}

bool Mode::verifyChannelSyntax(Client* sender, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
    if (channelName[0] != '#') {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
        return false;
    }
	return true;
}

bool Mode::validateChannelExists(Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server, std::string& channelName, const std::string& errorMessage) {
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it == channels.end()) {
		sendError(sender, ":" + server->getServerName() + " 403 " + sender->getNickname() + " " + channelName + errorMessage);
		return;
	}
}

void Mode::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
    (void)clients;
    (void)server;

    if (isParamCountValid(sender, command, server, 1, " MODE :Not enough parameters\r\n") == false) {
        return;
    }

    std::string channelName = command.getParam(0);
	if (verifyChannelSyntax(sender, server, channelName, " :No such channel\r\n") == false) {
		return ;
	}

    if (validateChannelExists(sender, channels, server, channelName, " :No such channel\r\n") == false) {
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
}

void Mode::modifyInviteOnlyMode(Channel* channel, MODEOPERATION operation){
    if (operation == MODE_ADD) {
        channel->setChannelMode(Channel::INVITE_ONLY);
    } else {
        channel->unsetChannelMode(Channel::INVITE_ONLY);
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
        channel->setUserLimit(std::stold(param));
    } else {
        channel->unsetChannelMode(Channel::USER_LIMIT);
        channel->setUserLimit(0);
    }
}