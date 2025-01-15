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
//     uintptr_t ident;        /* identifier for this event */
//     int16_t   filter;       /* filter for event */
//     uint16_t  flags;        /* action flags for kqueue */
//     uint32_t  fflags;       /* filter flag value */
//     intptr_t  data;         /* filter data value */
//     void      *udata;       /* opaque user data identifier */
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
			// else
			// 	handleClientEvent(events[i].ident);
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

	Client newClient(clientSocketFd, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	clients[clientSocketFd] = &newClient;
	std::cout << "New client connected : " << newClient.getIp() << ":" << newClient.getPort() << std::endl;
}