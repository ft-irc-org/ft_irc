#include "./server/Server.hpp"
#include "./server/ServerConfig.hpp"
#include <iostream>
#include <stdexcept>
#include <csignal>

// Signal handler to gracefully shut down the server
void signalHandler(int signum) {
    std::cout << "\nSignal (" << signum << ") received. Cleaning up...\n";
    exit(signum);
}

int main(int argc, char *argv[]) {
    try {
        // Register signal handler for Ctrl+C
        signal(SIGINT, signalHandler);

        // Check command line arguments
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
            return 1;
        }

        // Parse port number
        int port = std::stold(argv[1]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Error: Invalid port number. Port must be between 1 and 65535." << std::endl;
            return 1;
        }

        // Create server configuration
        ServerConfig config(port, argv[2]);

        // Initialize server
        std::cout << "Initializing server on port " << port << "..." << std::endl;
        Server server(config);

        // Start server
        std::cout << "Starting server..." << std::endl;
        server.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
