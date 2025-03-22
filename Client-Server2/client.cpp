#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_ADDR "127.0.0.1"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket creating error." << std::endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Can't connect the server." << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Connected. Enter 'exit' for leaving ." << std::endl;
    
    std::string message;
    while (true) {
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }

        if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
            std::cerr << "Message sanding error." << std::endl;
            break;
        }
    }

    close(sockfd);
    return 0;
}
