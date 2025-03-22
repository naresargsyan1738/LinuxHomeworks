#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>

#define SERVER_PORT 8080
#define MAX_CLIENTS 10

std::mutex mtx;  
void handle_client(int clientSock) {
    char buffer[1024];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break; 
        }
        mtx.lock();
        std::cout << "Message from client: " << std::string(buffer, bytesRead) << std::endl;
        mtx.unlock();
    }

    close(clientSock);
}

int main() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Socket creating error." << std::endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Socket connection error." << std::endl;
        close(serverSock);
        return -1;
    }

    if (listen(serverSock, MAX_CLIENTS) < 0) {
        std::cerr << "Listening error." << std::endl;
        close(serverSock);
        return -1;
    }

    std::cout << "Server is on. Waiting for clients ..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientLen);
        if (clientSock < 0) {
            std::cerr << "Accepting error." << std::endl;
            continue;
        }

        std::cout << "Client is connected." << std::endl;
        std::thread clientThread(handle_client, clientSock);
        clientThread.detach(); 
    }

    close(serverSock);
    return 0;
}
