#include "Whois.hpp"

Whois::Whois() {
}

Whois::~Whois() {
}

void Whois::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void)auth;

    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 431 " + sender->getNickname() + " :No nickname given\r\n";
        sender->setOutBuffer(response);
        return;
    }

    // target 파라미터가 있는지 확인
    std::string targetNickname;
    if (command.getParamCount() > 1) {
        targetNickname = command.getParam(1);
    } else {
        targetNickname = command.getParam(0);
    }

    // 대상 클라이언트 찾기
    Client *target = NULL;
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second->getNickname() == targetNickname) {
            target = it->second;
            break;
        }
    }

    if (target == NULL) {
        std::string response = ":" + server->getServerName() + " 401 " + sender->getNickname() + " " + targetNickname + " :No such nick/channel\r\n";
        sender->setOutBuffer(response);
        return;
    }

    // RPL_WHOISUSER (311)
    std::string response = ":" + server->getServerName() + " 311 " + sender->getNickname() + " " 
                        + target->getNickname() + " ~" + target->getNickname() + " " 
                        + target->getIp() + " * :" + target->getRealname() + "\r\n";
    sender->setOutBuffer(response);

    // RPL_WHOISSERVER (312)
    response = ":" + server->getServerName() + " 312 " + sender->getNickname() + " "
             + target->getNickname() + " " + server->getServerName() + " :IRC Server\r\n";
    sender->setOutBuffer(response);

    // RPL_WHOISCHANNELS (319)
    std::string channelList;
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = it->second;
        if (channel->isMember(target)) {
            std::string prefix = "";
            if (channel->isOperator(target)) prefix = "@";
        }
    }
    if (!channelList.empty()) {
        response = ":" + server->getServerName() + " 319 " + sender->getNickname() + " "
                 + target->getNickname() + " :" + channelList + "\r\n";
        sender->setOutBuffer(response);
    }

    // RPL_ENDOFWHOIS (318)
    response = ":" + server->getServerName() + " 318 " + sender->getNickname() + " "
             + target->getNickname() + " :End of /WHOIS list\r\n";
    sender->setOutBuffer(response);
}