#include "Ping.hpp"

Ping::Ping() {
}

Ping::~Ping() {
}

void Ping::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) channels;
    (void) auth;

    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " PING :Not enough parameters\r\n";
        sender->setOutBuffer(response);
        return;
    }

    // token을 포함한 PONG 응답
    std::string token = command.getParam(0);
    std::string response = ":" + server->getServerName() + " PONG " + server->getServerName() + " :" + token + "\r\n";
    sender->setOutBuffer(response);
}

//ping, pong <- server