#include "Mode.hpp"

Mode::Mode() {
}

Mode::~Mode() {
}

void Mode::execute(Client* sender, const Message& command, 
                  std::map<int, Client*> &clients, 
                  std::map<std::string, Channel*>& channels, 
                  Auth &auth) {
	(void) clients;
    if (command.getParamCount() < 1) {
        return sendError(sender, "461 MODE :Not enough parameters");
    }

    std::string target = command.getParam(0);
    
    if (command.getParamCount() == 1) {
        handleModeQuery(sender, target, channels);
        return;
    }

    std::string modes = command.getParam(1);
    std::string param = command.getParamCount() > 2 ? command.getParam(2) : "";

    if (target[0] == '#' && (auth.hasPermission(sender->getNickname(), target, Auth::CHANNEL_MODE) || auth.isOperator(sender->getNickname(), target))) {
        handleChannelMode(sender, target, modes, param, channels, auth);
    }
}

void Mode::handleModeQuery(Client* sender, const std::string& target, 
                          std::map<std::string, Channel*>& channels) {
    if (target[0] == '#') {
        std::map<std::string, Channel*>::iterator it = channels.find(target);
        if (it == channels.end()) {
            return sendError(sender, "403 " + target + " :No such channel");
        }
        
        std::string response = std::string(":") + sender->getNickname() + " MODE " + target + " " + getModeString(it->second) + "\r\n";
        // send(sender->getSocketFd(), response.c_str(), response.length(), 0);
        sender->setOutBuffer(response);
    } else {
        sendError(sender, "502 :Cant change mode for other users");
    }
}

void Mode::handleChannelMode(Client* sender, const std::string& channelName, const std::string& modes, const std::string& param, std::map<std::string, Channel*>& channels, Auth& auth) {
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        return sendError(sender, "403 " + channelName + " :No such channel");
    }

    Channel* channel = it->second;

    if (!auth.hasPermission(sender->getNickname(), channelName, Auth::CHANNEL_MODE)) {
        return sendError(sender, "482 " + channelName + " :You're not channel operator :: MODE");
    }

    bool adding = true;
    std::string appliedModes;
    std::string appliedParams;

    for (std::string::const_iterator mode_it = modes.begin(); 
         mode_it != modes.end(); ++mode_it) {
        char mode = *mode_it;

        if (mode == '+') {
            adding = true;
            continue;
        }
        if (mode == '-') {
            adding = false;
            continue;
        }

        switch (mode) {
            case 'i': // invite-only
                if (adding) {
                    channel->setChannelMode(Channel::INVITE_ONLY);
                } else {
                    channel->unsetChannelMode(Channel::INVITE_ONLY);
                }
                appliedModes += mode;
                break;

            case 't': // topic restriction
                if (adding) {
                    channel->setChannelMode(Channel::TOPIC_RESTRICTED);
                } else {
                    channel->unsetChannelMode(Channel::TOPIC_RESTRICTED);
                }
                appliedModes += mode;
                break;

            case 'k': // channel key (password)
                if (adding && param.empty()) {
                    sendError(sender, "461 MODE +k :Need key parameter");
                    continue;
                }
                if (adding) {
                    channel->setPassword(param);
                    channel->setChannelMode(Channel::PRIVATE);
                } else {
                    channel->setPassword("");
                    channel->unsetChannelMode(Channel::PRIVATE);
                }
                appliedModes += mode;
                if (adding) appliedParams = param;
                break;

            case 'o': // operator status
                if (param.empty()) {
                    sendError(sender, "461 MODE +o :Need nickname parameter");
                    continue;
                }
                if (adding) {
                    auth.grantOperator(param, channelName);
                } else {
                    auth.revokeOperator(param, channelName);
                }
                appliedModes += mode;
                appliedParams = param;
                break;

            case 'l': // user limit
                if (adding) {
                    if (param.empty()) {
                        sendError(sender, "461 MODE +l :Need limit parameter");
                        continue;
                    }
                    // Convert string to integer
                    int limit = 0;
                    std::istringstream iss(param);
                    if (!(iss >> limit) || limit <= 0) {
                        sendError(sender, "461 MODE +l :Invalid limit parameter");
                        continue;
                    }
                    channel->setUserLimit(limit);
                    channel->setChannelMode(Channel::USER_LIMIT);
                } else {
                    channel->setUserLimit(0);
                    channel->unsetChannelMode(Channel::USER_LIMIT);
                }
                appliedModes += mode;
                if (adding) appliedParams = param;
                break;

            default:
                sendError(sender, "472 " + std::string(1, mode) + " :Unknown mode");
                break;
        }
    }

    // 변경된 모드가 있을 경우에만 브로드캐스트
    if (!appliedModes.empty()) {
        std::string notification = std::string(":") + sender->getNickname() + 
                                 " MODE " + channelName + " " + 
                                 (adding ? "+" : "-") + appliedModes;
        
        if (!appliedParams.empty()) {
            notification += " " + appliedParams;
        }
        notification += "\r\n";
        
        channel->broadcast(notification, sender);
    }
}

std::string Mode::getModeString(Channel* channel) {
	std::string modeString;
	if (channel->getChannelMode() & Channel::INVITE_ONLY) {
		modeString += "i";
	}
	if (channel->getChannelMode() & Channel::OPERATOR_PRIVILEGES) {
		modeString += "o";
	}
	if (channel->getChannelMode() & Channel::TOPIC_RESTRICTED) {
		modeString += "t";
	}
	if (channel->getChannelMode() & Channel::PRIVATE) {
		modeString += "p";
	}
	return modeString;
}

