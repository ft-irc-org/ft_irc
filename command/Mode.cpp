#include "Mode.hpp"

Mode::Mode() {
}

Mode::~Mode() {
}

void Mode::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server){
    (void)clients;
    (void)server;

	if (isParamCountValid(sender, command, server, 1, "461", " MODE :Not enough parameters\r\n") == false) {
        return;
    }

    std::string channelName = command.getParam(0);
	if (verifyChannelSyntax(sender, server, channelName, "403", " :No such channel\r\n") == false) {
		return ;
	}

    if (validateChannelExists(sender, channels, server, channelName, "403", " :No such channel\r\n") == false) {
        return;
    }

    Channel* channel = channels[channelName];
    if (!auth.isOperator(sender->getNickname(), channelName) || !auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
        std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + " " + channelName + " :You're not channel operator :: MODE\r\n";
        sender->setOutBuffer(response);
        return;
    }

   if (isParamCountValid(sender, command, server, 2, "461", " MODE :Not enough parameters\r\n") == false) {
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
			if (isParamCountValid(sender, command, server, 3, "461", " MODE :Not enough parameters\r\n") == false) {
				return;
			}
            modifyKeyMode(channel, operation, command.getParam(2));
            break;
        case CAHNNELMODE_OPERATOR:
            if (isParamCountValid(sender, command, server, 3, "461", " MODE :Not enough parameters\r\n") == false) {
				return;
			}
            modifyOperatorMode(auth, channel, operation, command.getParam(2));
            break;
        case CAHNNELMODE_LIMIT:
            if (isParamCountValid(sender, command, server, 3, "461", " MODE :Not enough parameters\r\n") == false) {
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