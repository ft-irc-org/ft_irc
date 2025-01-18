#include "Server.hpp"

Server::Server(const ServerConfig& config): config(config) {
	std::cout << "Server created" << std::endl;
	std::cout << "Server port : " << config.getPort() << std::endl;

	// Create a socket
	if ((serverSocketFd = initSocket()) < 0) throw std::runtime_error(": Error while creating a socket!");
	if ((kqueueFd = initKqueue()) < 0) throw std::runtime_error(": Error while creating a kqueue!");
}

Server::~Server() {}

int Server::initSocket() {
	// socket openning
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET : IPv4 인터넷 프로토콜, SOCK_STREAM : TCP 통신, 0 : 프로토콜 번호
	if (serverSocket < 0)
		throw std::runtime_error("Error while opening a socket!");
 
	// set socket option
	int opt = 1;
	// SOL_SOCKET : 소켓 옵션 레벨, SO_REUSEADDR : 주소 재사용, opt : 옵션 값 (1: enable), sizeof(opt) : 옵션 값의 크기
	// 주소 재사용 옵션은 TIME_WAIT 상태(불안정 종료 등으로 인해 socket이 time wait에 걸릴 때가 있음)의 소켓이 포트를 사용할 수 있도록 해준다.
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error while setting socket options!");

	// non-blocking socket
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error while setting socket to non-blocking!");
	
	// socket address setting
	struct sockaddr_in serverAddress; // IPv4 인터넷 프로토콜용 소켓 주소 구조체
	memset(&serverAddress, 0, sizeof(serverAddress)); // 구조체 변수 초기화
	serverAddress.sin_family = AF_INET; // 주소 체계(Address Family)를 설정
	serverAddress.sin_addr.s_addr = INADDR_ANY; // IP 주소를 설정, INADDR_ANY : 모든 IP 주소를 의미, 모든 IP로부터의 연결을 허용
	serverAddress.sin_port = htons(config.getPort()); // 포트 번호를 설정, htons : host to network short, network byte로 변환

	// binding
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Error while binding socket!");

	// listening
	if (listen(serverSocket, MAX_CLIENTS) < 0)
		throw std::runtime_error("Error while listening socket!");

	return serverSocket;
}

int Server::initKqueue() {
	kqueueFd = kqueue();
	if (kqueueFd < 0) 
		throw std::runtime_error("Error while creating kqueue!");
	memset(events, 0, sizeof(events));
	return kqueueFd;
}

// struct kevent {
//	 uintptr_t ident;		/* identifier for this event */
//	 int16_t   filter;	   /* filter for event */
//	 uint16_t  flags;		/* action flags for kqueue */
//	 uint32_t  fflags;	   /* filter flag value */
//	 intptr_t  data;		 /* filter data value */
//	 void	  *udata;	   /* opaque user data identifier */
// };

void Server::start() {
	std::cout << "Server started at port : " << config.getPort() << std::endl;
	struct kevent event;
	// &event: 이벤트 정보를 저장할 구조체
	// serverSocketFd: 감시할 파일 디스크립터(서버 소켓)
	// EVFILT_READ: 읽기 가능 상태를 감시
	// EV_ADD: 이 이벤트를 kqueue에 추가하라는 의미
	// 0, 0, NULL: 추가 설정 없음
	EV_SET(&event, serverSocketFd, EVFILT_READ, EV_ADD, 0, 0, NULL); // server socket을 kqueue에 등록
	if (kevent(kqueueFd, &event, 1, NULL, 0, NULL) == -1)
		throw std::runtime_error("Failed to add server socket to kqueue");

	while (true) {
		// event 대기
		int numEvents = kevent(kqueueFd, NULL, 0, events, MAX_CLIENTS, NULL);
		if (numEvents < 0) throw std::runtime_error("Error while polling events!");
		for (int i = 0; i < numEvents; i++) {
			// server socket에 이벤트가 발생했을 때 (신규 클라이언트 접속)
			if (events[i].ident == serverSocketFd)
				acceptClient();
			else
				handleClientEvent(events[i].ident);
		}
	}
}

void Server::acceptClient(){
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	memset(&clientAddress, 0, sizeof(clientAddress));

	// create client socket
	int clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (clientSocketFd < 0)
		throw std::runtime_error("Error while accepting client!");

	// set client socket to non-blocking
	if (fcntl(clientSocketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error while setting client socket to non-blocking!");

	// add client socket to kqueue
	struct kevent event;
	EV_SET(&event, clientSocketFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kqueueFd, &event, 1, NULL, 0, NULL) == -1)
		throw std::runtime_error("Failed to add client socket to kqueue!");

	clients[clientSocketFd] = new Client(clientSocketFd, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	std::cout << "New client connected : " << clients[clientSocketFd]->getIp() << ":" << clients[clientSocketFd]->getPort() << std::endl;

	// send authentication message
	std::string welcomeMessage = "Please authenticate using PASS command\r\n";
	std::string response = ":localhost 451 :You must first authenticate using PASS command\r\n";
    send(clientSocketFd, response.c_str(), response.size(), 0);
}

void Server::handleClientEvent(int clientSocketFd) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytesRead = recv(clientSocketFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        Client* client = clients[clientSocketFd];
        std::string receivedData(buffer, bytesRead);
        
        // Split messages by CRLF and process each command
        size_t start = 0;
        size_t end = 0;
        
        while ((end = receivedData.find("\r\n", start)) != std::string::npos) {
            std::string command = receivedData.substr(start, end - start);
            if (!command.empty()) {
				std::cout << "processing command : " << command << std::endl;
                if (command.find("PASS") != std::string::npos && !client->isAuthenticated()) {
                    std::string password = command.substr(command.find("PASS") + 5);
                    if (password == config.getPassword()) {
                        client->setAuthentication(true);
                        std::string response = ":localhost 001 :Password accepted\r\n";
                        send(clientSocketFd, response.c_str(), response.size(), 0);
                    } else {
                        std::string response = ":localhost 464 :Password incorrect\r\n";
                        send(clientSocketFd, response.c_str(), response.size(), 0);
                    }
                }
                // NICK 처리
                else if (command.find("NICK") != std::string::npos) {
                    size_t space_pos = command.find(' ');
                    if (space_pos != std::string::npos) {
                        std::string nickname = command.substr(space_pos + 1);
                        client->setNickname(nickname);
						std::string response = ":localhost 001 " + nickname + " :Welcome to the IRC network\r\n";
						send(clientSocketFd, response.c_str(), response.size(), 0);
                        // NICK 응답은 USER 명령어가 올 때까지 대기
                    }
                }
                // USER 처리 (NICK이 설정된 경우에만)
                else if (command.find("USER") != std::string::npos && !client->getNickname().empty()) {
                    std::string nick = client->getNickname();
                    std::string welcomeResponse = 
                        ":localhost 001 " + nick + " :Welcome to the IRC network\r\n"
                        ":localhost 002 " + nick + " :Your host is localhost, running version 1.0\r\n"
                        ":localhost 003 " + nick + " :This server was created today\r\n"
                        ":localhost 004 " + nick + " localhost 1.0 o o\r\n";
                    send(clientSocketFd, welcomeResponse.c_str(), welcomeResponse.size(), 0);
                }
				else if (command.find("WHOIS") != std::string::npos) {
					size_t space_pos = command.find(' ');
					if (space_pos != std::string::npos) {
						std::string nickname = command.substr(space_pos + 1);
						std::string response = ":localhost 311 " + client->getNickname() + " " + nickname + " ~user localhost * :" + nickname + "\r\n";
						send(clientSocketFd, response.c_str(), response.size(), 0);
					}
				}
				else if (command.find("JOIN") != std::string::npos) {
					size_t space_pos = command.find(' ');
					if (space_pos != std::string::npos) {
						std::string channel = command.substr(space_pos + 1);
						std::string response = ":" + client->getNickname() + " JOIN :" + channel + "\r\n";
						send(clientSocketFd, response.c_str(), response.size(), 0);
					}
				}
                // PING 처리
                else if (command.find("PING") != std::string::npos) {
    				// PING 메시지의 전체 형식을 로그로 출력
    				std::cout << "PING message received: [" << command << "]" << std::endl;
    
    				std::string response;
    				size_t space_pos = command.find(' ');
    				if (space_pos != std::string::npos) {
    				    // PING 다음의 모든 텍스트를 서버 파라미터로 사용
    				    std::string server = command.substr(space_pos + 1);
    				    response = "PONG :" + server + "\r\n";
    				} else {
    				    // 파라미터가 없는 경우 기본값 사용
    				    response = "PONG :localhost\r\n";
    				}
    
				    // 응답 전송을 로그로 출력
				    std::cout << "Sending PONG response: [" << response << "]" << std::endl;
				    send(clientSocketFd, response.c_str(), response.size(), 0);
				}
                // MODE 처리
                else if (command.find("MODE") != std::string::npos) {
                    std::string response = ":localhost MODE " + client->getNickname() + " :+i\r\n";
                    send(clientSocketFd, response.c_str(), response.size(), 0);
                }
                // 알 수 없는 명령어
                else if (!command.empty()) {
                    std::string response = ":localhost 421 " + client->getNickname() + " :Unknown command\r\n";
                    send(clientSocketFd, response.c_str(), response.size(), 0);
                }
            }
            start = end + 2; // Skip \r\n
        }
    } else if (bytesRead == 0) {
        std::cout << "Client disconnected : " << clients[clientSocketFd]->getIp() << ":" 
                 << clients[clientSocketFd]->getPort() << std::endl;
        delete clients[clientSocketFd];
        clients.erase(clientSocketFd);
        close(clientSocketFd);
    } else {
        std::cerr << "Error receiving from client\n";
        delete clients[clientSocketFd];
        clients.erase(clientSocketFd);
        close(clientSocketFd);
    }
}