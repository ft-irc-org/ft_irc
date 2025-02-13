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
        return;  // PONG은 에러 응답을 보내지 않음
    }

    // 클라이언트는 서버 파라미터를 보내면 안됨
    std::string token = command.getParam(command.getParamCount() - 1);  // 마지막 파라미터가 token
    
    sender->updateLastPingTime();
    sender->setAwaitingPong(false);
}