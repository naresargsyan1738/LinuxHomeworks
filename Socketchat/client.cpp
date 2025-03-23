#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creating error." << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection error." << std::endl;
        close(sockfd);
        return -1;
    }

    char buffer[BUFFER_SIZE];

    int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    std::cout << buffer << std::endl;

    while (true) {
        std::cout << "Enter the message: ";
        std::string message;
        std::getline(std::cin, message);

        if (message == "/exit") {
            send(sockfd, message.c_str(), message.length(), 0);
            break;
        }
       
        else if (message == "/list") {
            send(sockfd, message.c_str(), message.length(), 0);
            bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
            buffer[bytes_received] = '\0';
            std::cout << buffer << std::endl;
        } else {
            send(sockfd, message.c_str(), message.length(), 0);
        }

        bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        buffer[bytes_received] = '\0';
        std::cout << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}
