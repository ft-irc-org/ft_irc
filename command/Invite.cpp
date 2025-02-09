#include "Invite.hpp"

Invite::Invite() {
}

Invite::~Invite() {
}

void Invite::execute(Client* sender, const Message& command, std::map<int, Client*> &clients, std::map<std::string, Channel*>& channels, Auth &auth, ServerEventHandler *server) {    
   (void)auth;

   if (command.getParamCount() < 2) {
       std::string response = ":" + server->getServerName() + " 461 " + sender->getNickname() + 
                            " INVITE :Not enough parameters\r\n";
       sender->setOutBuffer(response);
       return;
   }

   std::string nickname = command.getParam(0);
   std::string channelName = command.getParam(1);
   
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

   if (!channel->searchMember(sender->getNickname())) {
       std::string response = ":" + server->getServerName() + " 442 " + sender->getNickname() + 
                            " " + channelName + " :You're not on that channel\r\n";
       sender->setOutBuffer(response);
       return;
   }

   if (channel->hasMode(Channel::INVITE_ONLY)) {
       if (!auth.hasPermission(sender->getNickname(), channelName, Auth::OP)) {
           std::string response = ":" + server->getServerName() + " 482 " + 
                                sender->getNickname() + " " + channelName + 
                                " :You're not channel operator\r\n";
           sender->setOutBuffer(response);
           return;
       }
   }

   std::map<int, Client*>::iterator it2 = clients.begin();
   for ( ; it2 != clients.end(); ++it2) {
       if (it2->second->getNickname() == nickname) {
           break;
       }
   }
   if (it2 == clients.end()) {
       std::string response = ":" + server->getServerName() + " 401 " + sender->getNickname() + 
                            " " + nickname + " :No such nick/channel\r\n";
       sender->setOutBuffer(response);
       return;
   }

   Client* target = it2->second;
   
   if (channel->searchMember(nickname)) {
       std::string response = ":" + server->getServerName() + " 443 " + sender->getNickname() + 
                            " " + nickname + " " + channelName + " :is already on channel\r\n";
       sender->setOutBuffer(response);
       return;
   }

   std::string response = ":" + server->getServerName() + " 341 " + sender->getNickname() + 
                        " " + nickname + " " + channelName + "\r\n";
   sender->setOutBuffer(response);
   response = ":" + sender->getNickname() + " INVITE " + nickname + " " + channelName + "\r\n";
   target->setOutBuffer(response);

    channel->addWhiteList(target);
}