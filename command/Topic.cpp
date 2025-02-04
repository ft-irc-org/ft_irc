#include "Topic.hpp"

Topic::Topic() {
}

Topic::~Topic() {
}

void Topic::execute(Client* sender, const Message& command, 
                   std::map<int, Client*> &clients, 
                   std::map<std::string, Channel*>& channels, 
                   Auth &auth, ServerEventHandler *server) {
    (void)clients;
    (void)server;
    // 디버깅 출력 추가
    std::cout << "Parameter count: " << command.getParamCount() << std::endl;
    for (size_t i = 0; i < command.getParamCount(); i++) {
        std::cout << "Param " << i << ": " << command.getParam(i) << std::endl;
    }

    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + 
                             " TOPIC :Not enough parameters\r\n";
        sender->setOutBuffer(response);
        return;
    }

    std::string channelName = command.getParam(0);
    
    if (channelName[0] != '#') {
        std::string response = ":" + server->getServerName() + " 403 " + sender->getNickname() + 
                             " " + channelName + " :No such channel\r\n";
        sender->setOutBuffer(response);
        return;
    }

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        std::string response = ":" + server->getServerName() + " 403 " + sender->getNickname() + 
                             " " + channelName + " :No such channel\r\n";
        sender->setOutBuffer(response);
        return;
    }

    Channel* channel = it->second;
    std::cout << "Channel found: " << channel->getChannelName() << std::endl;

    // 토픽 조회
if (command.getParamCount() == 1) {
    std::string response;
    if (channel->getTopic().empty()) {
        response = ":331 " + sender->getNickname() + " " +
                   channelName + " :No topic is set\r\n";
    } else {
        // 1. 현재 토픽 내용 전송
        response = ":332 " + sender->getNickname() + " " + 
                   channelName + " :" + channel->getTopic() + "\r\n";
        sender->setOutBuffer(response);
    }
    sender->setOutBuffer(response);
    return;
}

    // 토픽 설정
    if (channel->hasMode(Channel::TOPIC_RESTRICTED)) {
        if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
            std::string response = ":" + server->getServerName() + " 482 " + sender->getNickname() + 
                                 " " + channelName + " :You're not channel operator\r\n";
            sender->setOutBuffer(response);
            return;
        }
    }

    std::string newTopic = command.getParam(1);
    channel->setTopic(newTopic);

// 1. 토픽 변경 알림
std::string notification = ":" + sender->getNickname() + "!" + sender->getRealname() + 
                         "@" + server->getServerName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
sender->setOutBuffer(notification);
channel->broadcast(notification, sender);

// 2. 채널의 모든 클라이언트에게 토픽 업데이트 정보 전송
std::string topicMsg = ":332 " + sender->getNickname() + " " + 
                      channelName + " :" + newTopic + "\r\n";
sender->setOutBuffer(topicMsg);
channel->broadcast(topicMsg, sender);

// 3. 토픽 설정자와 시간 정보도 전송 (이게 클라이언트 캐시 업데이트의 핵심)
std::string whoTimeMsg = ":333 " + sender->getNickname() + " " + 
                        channelName + " " + sender->getNickname() + " " +
                        sender->getRealname() + "@" + server->getServerName() + " " + 
                        std::to_string(time(NULL)) + "\r\n";
sender->setOutBuffer(whoTimeMsg);
channel->broadcast(whoTimeMsg, sender);
}
