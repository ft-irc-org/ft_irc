#include "Notice.hpp"

Notice::Notice() {
}

Notice::~Notice() {
}

void Notice::execute(Client* sender, const Message& command, 
				  std::map<int, Client*> &clients, 
				  std::map<std::string, Channel*>& channels, 
				  Auth &auth, ServerEventHandler *server) {
	(void)channels;
	(void)auth;
	(void)server;

	if (command.getParamCount() < 2) {
        return;
    }

    std::string target = command.getParam(0);
    std::string message = command.getParam(1);
	
    std::string notice = ":" + sender->getNickname() + " NOTICE " + target + " :" + message + "\r\n";

    // 채널 메시지 처리
    if (target[0] == '#') {
        std::map<std::string, Channel*>::iterator chanIt = channels.find(target);
        if (chanIt != channels.end()) {
            Channel* channel = chanIt->second;
            
            // 발신자가 채널 멤버인지 확인
            if (!channel->isMember(sender)) {
				std::string response = ":" + server->getServerName() + " 442 " + sender->getNickname() + " " + target + " :You're not on that channel\r\n";
                return;  // 멤버가 아니면 메시지를 보낼 수 없음
            }

            // 채널의 모든 멤버에게 메시지 전송 (sender 제외)
            channel->broadcast(notice, sender, server);
        }
        return;
    }

    // 개인 메시지 처리
    for (std::map<int, Client*>::iterator it = clients.begin(); 
         it != clients.end(); ++it) {
        if (it->second->getNickname() == target) {
            it->second->setOutBuffer(notice);
            return;
        }
    }
}