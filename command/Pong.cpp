#include "Pong.hpp"

Pong::Pong() {
}

Pong::~Pong() {
}

void Pong::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {
    (void) clients;
    (void) channels;
    (void) auth;
    (void) server;

    if (command.getParamCount() < 1) {
        std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + " PONG :Not enough parameters\r\n";
        sender->setOutBuffer(response);
        return;
    }

    // PONG을 받으면 클라이언트의 상태 업데이트
    sender->updateLastPingTime();
    sender->setAwaitingPong(false);
}