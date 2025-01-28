#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::map<std::string, Channel*>& channels, std::map<int, Client*>& clients, const ServerConfig& config, ServerEventHandler *server) : auth(config), channels(channels), clients(clients), server(server) {
	registerHandler("JOIN", new Join());
	registerHandler("KICK", new Kick());
	registerHandler("MODE", new Mode());
	registerHandler("NICK", new Nick());
	registerHandler("PASS", new Pass());
	registerHandler("PING", new Ping());
	registerHandler("PONG", new Pong());
	registerHandler("PRIVMSG", new Privmsg());
	registerHandler("QUIT", new Quit());
	registerHandler("USER", new User());
	registerHandler("WHOIS", new Whois());
	registerHandler("PART", new Part());
}

Dispatcher::~Dispatcher() {
}

void Dispatcher::registerHandler(const std::string& command, CommandHandler* handler) {
	handlers[command] = handler;
}

void Dispatcher::dispatch(Client* client, const Message& command) {
	if (command.getVerb() == "CAP") {
		return ;
	}
	if (!client->isAuthenticated() && command.getVerb() != "PASS" && command.getVerb() != "NICK" && command.getVerb() != "USER") {
		std::string response = ":localhost 451 " + client->getNickname() + " :You have not registered\r\n";
		// send(client->getSocketFd(), response.c_str(), response.size(), 0);
		client->setOutBuffer(response);
		return;
	}

	std::map<std::string, CommandHandler*>::iterator it = handlers.find(command.getVerb());
	if (it != handlers.end()) {
		it->second->execute(client, command, clients, channels, auth, server);
	} else {
		std::string response = ":localhost 421 " + client->getNickname() + " :Unknown command\r\n";
		// send(client->getSocketFd(), response.c_str(), response.size(), 0);
		client->setOutBuffer(response);
	}
}