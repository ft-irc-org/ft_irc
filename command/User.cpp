#include "User.hpp"

User::User() {
}

User::~User() {
}

void User::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth) {
    (void)clients;
    (void)channels;
    (void)auth;

	if (command.getParamCount() < 4) {
        return sendError(sender, "461 " + sender->getNickname() + " USER :Not enough parameters");
    }

    if (sender->isUserAuthenticated()) {
        return sendError(sender, "462 " + sender->getNickname() + " :Already registered");
    }
    // std::cout << "nickname : " << sender->getNickname() << std::endl;
    std::string username = command.getParam(0);
    std::string realname = command.getParam(3);

    sender->setRealname(realname);
    sender->setUserAuthenticated(true);

    if (!sender->getNickname().empty()) {
        // std::cout << "nickname : " << sender->getNickname() << std::endl;
        sender->setAuthentication(true);
        std::string welcome = ":localhost 001 " + sender->getNickname() + " :Welcome to IRC\r\n";
        // send(sender->getSocketFd(), welcome.c_str(), welcome.size(), 0);
        sender->setOutBuffer(welcome);
        std::string yourHost = ":localhost 002 " + sender->getNickname() + " :Your host is localhost\r\n";
        // send(sender->getSocketFd(), yourHost.c_str(), yourHost.size(), 0);
        sender->setOutBuffer(yourHost);
        std::string created = ":localhost 003 " + sender->getNickname() + " :This server was created on 2021-06-01\r\n";
        // send(sender->getSocketFd(), created.c_str(), created.size(), 0);
        sender->setOutBuffer(created);
        std::string myInfo = ":localhost 004 " + sender->getNickname() + " localhost 0.0.1 o o\r\n";
        // send(sender->getSocketFd(), myInfo.c_str(), myInfo.size(), 0);
        sender->setOutBuffer(myInfo);
        std::string isSupport = ":localhost 005 " + sender->getNickname() + " PREFIX=(ov)@+ CHANTYPES=#& :are supported by this server\r\n";
        // send(sender->getSocketFd(), isSupport.c_str(), isSupport.size(), 0);
        sender->setOutBuffer(isSupport);
        std::string motd = ":localhost 376 " + sender->getNickname() + " :End of /MOTD command\r\n";
        // send(sender->getSocketFd(), motd.c_str(), motd.size(), 0);
        sender->setOutBuffer(motd);
    }
}