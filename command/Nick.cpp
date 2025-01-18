#include "Nick.hpp"

Nick::Nick() {
}

Nick::~Nick() {
}

void Nick::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
	if (command.getParamCount() < 1) {
		return sendError(sender, "461 NICK :Not enough parameters");
	}
	

	std::string newNickname = command.getParam(0);

	if (!isValidNickname(newNickname)) {
        return sendError(sender, "432 " + newNickname + " :Erroneous nickname");
    }

	for (std::map<int, Client*>::iterator it = clients.begin(); 
         it != clients.end(); ++it) {
        if (it->second->getNickname() == newNickname) {
            return sendError(sender, "433 " + newNickname + 
                           " :Nickname is already in use");
        }
    }

	std::map<int, Client*>::iterator it = clients.find(sender->getSocketFd());

	if (it == clients.end()) {
		return sendError(sender, "401 " + newNickname + " :No such nick/channel");
	}

	Client* client = it->second;
	std::string oldNickname = client->getNickname();

	if (oldNickname.empty()) {
        client->setNickname(newNickname);
        std::string response = ":localhost 001 " + newNickname + 
                             " :Welcome to the Internet Relay Network\r\n";
        send(client->getSocketFd(), response.c_str(), response.size(), 0);
        return;
    }

	client->setNickname(newNickname);

	auth.updateNickname(oldNickname, newNickname);
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;
		if (channel->searchMember(oldNickname) != NULL) {
			channel->removeMember(*client);
			channel->addMember(*client);
		}
	}

	std::string response = ":" + oldNickname + " NICK " + newNickname + "\r\n";
	broadcastToChannels(response, client, channels);
}

void Nick::broadcastToChannels(const std::string& message, Client* sender, std::map<std::string, Channel*>& channels) {
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;
		if (channel->searchMember(sender->getNickname()) != NULL) {
			channel->broadcast(message, *sender);
		}
	}
}

void Nick::sendError(Client* client, const std::string& error) {
	std::string response = ":localhost " + error + "\r\n";
	send(client->getSocketFd(), response.c_str(), response.size(), 0);
}

bool Nick::isValidNickname(const std::string& nickname) const {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }

    // 첫 글자는 알파벳이어야 함
    if (!isalpha(nickname[0])) {
        return false;
    }

    // 나머지 문자는 알파벳, 숫자, 특수문자(-_[]{}\|) 허용
    for (std::string::const_iterator it = nickname.begin() + 1; 
         it != nickname.end(); ++it) {
        if (!isalnum(*it) && strchr("-_[]{}\\|", *it) == NULL) {
            return false;
        }
    }
    return true;
}
