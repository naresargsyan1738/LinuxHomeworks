#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::string command;
    std::cout << "Enter operation (ADD, SUB, MUL, DIV) and two numbers: ";
    std::getline(std::cin, command);

    send(sock, command.c_str(), command.size(), 0);
    
    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread < 0) {
        std::cerr << "Read error" << std::endl;
        return -1;
    }

    std::cout << "Server response: " << buffer << std::endl;

    close(sock);
    return 0;
}

