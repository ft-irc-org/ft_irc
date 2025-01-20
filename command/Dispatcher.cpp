#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::map<std::string, Channel*>& channels, std::map<int, Client*>& clients) : channels(channels), clients(clients) {
	registerHandler("JOIN", new Join());
	registerHandler("KICK", new Kick());
	// registerHandler("PART", new Part());
	registerHandler("PRIVMSG", new Privmsg());
	registerHandler("NICK", new Nick());
	registerHandler("PING", new Ping());
	registerHandler("PONG", new Pong());
	registerHandler("MODE", new Mode());
	registerHandler("QUIT", new Quit());
}

Dispatcher::~Dispatcher() {
}

void Dispatcher::registerHandler(const std::string& command, CommandHandler* handler) {
	handlers[command] = handler;
}

void Dispatcher::dispatch(Client* client, const Message& command) {
	if (!client->isAuthenticated() && command.getVerb() != "PASS" && command.getVerb() != "NICK" && command.getVerb() != "USER") {
		std::string response = ":localhost 451 " + client->getNickname() + " :You have not registered\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
		return;
	}

	std::map<std::string, CommandHandler*>::iterator it = handlers.find(command.getVerb());
	if (it != handlers.end()) {
		it->second->execute(client, command, clients, channels, auth);
	} else {
		std::string response = ":localhost 421 " + client->getNickname() + " :Unknown command\r\n";
		send(client->getSocketFd(), response.c_str(), response.size(), 0);
	}
}