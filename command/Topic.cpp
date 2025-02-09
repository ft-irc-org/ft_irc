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

    // 토픽 조회 (매개변수가 1개일 때)
    if (command.getParamCount() == 1) {
        if (channel->getTopic().empty()) {
            std::string response = ":" + server->getServerName() + " 331 " + 
                                 sender->getNickname() + " " + channelName + 
                                 " :No topic is set\r\n";
            sender->setOutBuffer(response);
        } else {
            // 토픽 내용 전송
            std::string topicMsg = ":" + server->getServerName() + " 332 " + 
                                 sender->getNickname() + " " + channelName + 
                                 " :" + channel->getTopic() + "\r\n";
            sender->setOutBuffer(topicMsg);

            // 토픽 설정자와 시간 정보 전송
            std::string whoTimeMsg = ":" + server->getServerName() + " 333 " + 
                                   sender->getNickname() + " " + channelName + " " +
                                   channel->getTopicSetter() + " " + 
                                   std::to_string(channel->getTopicTime()) + "\r\n";
            sender->setOutBuffer(whoTimeMsg);
        }
        return;
    }

    // 토픽 설정 (매개변수가 2개일 때)
    if (channel->hasMode(Channel::TOPIC_RESTRICTED)) {
        if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
            std::string response = ":" + server->getServerName() + " 482 " + 
                                 sender->getNickname() + " " + channelName + 
                                 " :You're not channel operator\r\n";
            sender->setOutBuffer(response);
            return;
        }
    }

    std::string newTopic = command.getParam(1);
    // 토픽 설정자 정보를 포함하여 저장
    channel->setTopic(newTopic, 
                     sender->getNickname() + "!" + 
                     sender->getRealname() + "@" + 
                     server->getServerName());

    // 토픽 변경 알림을 채널의 모든 사용자에게 브로드캐스트
    // 토픽 내용 전송
    std::string topicMsg = ":" + server->getServerName() + " 332 " + 
                            sender->getNickname() + " " + channelName + 
                            " :" + channel->getTopic() + "\r\n";
    channel->broadcast(topicMsg, sender, "TOPIC");

    // 토픽 설정자와 시간 정보 전송
    std::string whoTimeMsg = ":" + server->getServerName() + " 333 " + 
                            sender->getNickname() + " " + channelName + " " +
                            channel->getTopicSetter() + " " + 
                            std::to_string(channel->getTopicTime()) + "\r\n";
    channel->broadcast(whoTimeMsg, sender, "TOPIC");
}
