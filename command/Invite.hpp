#ifndef INVITE_HPP
# define INVITE_HPP

#include <unistd.h>

# include "CommandHandler.hpp"

class Invite : public CommandHandler {
    public:
        Invite();
        ~Invite();
        void execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server);
};

#endif
