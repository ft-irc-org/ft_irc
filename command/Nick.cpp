#include "Nick.hpp"

Nick::Nick() {
}

Nick::~Nick() {
}

void Nick::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) server;
    if (command.getParamCount() < 1) {
        sender->setOutBuffer("461 " + sender->getNickname() + " NICK :Not enough parameters\r\n");
    }

    if (!sender->isPassAuthenticated()) {
        sender->setOutBuffer("451 " + sender->getNickname() + " :You must enter PASS first\r\n");
        return;
    }

	std::string newNickname = command.getParam(0);

	// ignore if the new nickname is the same as the old one
	// std::cout << "sender->getNickname() : " << sender->getNickname() << std::endl;
	// std::cout << "newNickname : " << newNickname << std::endl;
	if (sender->getNickname() == newNickname) {
        return;
    }

	if (!isValidNickname(newNickname)) {
        sender->setOutBuffer("432 " + newNickname + " :Erroneous nickname\r\n");
        return;
    }

	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second != sender && it->second->getNickname() == newNickname) {
            sender->setOutBuffer("433 " + newNickname + " :Nickname is already in use\r\n");
            return;
        }
    }

	std::string oldNickname = sender->getNickname();
    sender->setNickname(newNickname);

    if (!oldNickname.empty()) {
        auth.updateNickname(oldNickname, newNickname);
        for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
            Channel* channel = it->second;
            if (channel->searchMember(oldNickname) != NULL) {
                channel->removeMember(sender);
                channel->addMember(sender);
            }
        }
        std::string response = ":" + oldNickname + " NICK " + newNickname + "\r\n";
        broadcastToChannels(response, sender, channels, server);
    }
}

void Nick::broadcastToChannels(const std::string& message, Client* sender, std::map<std::string, Channel*>& channels, ServerEventHandler *server) {
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;
		if (channel->searchMember(sender->getNickname()) != NULL) {
			channel->broadcast(message, sender, server);
		}
	}
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
