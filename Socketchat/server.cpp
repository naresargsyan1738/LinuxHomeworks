#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

struct Client {
    int sockfd;
    std::string username;
};

std::vector<Client> clients; 
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(const std::string& message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (const auto& client : clients) {
        if (client.sockfd != sender_sock) {
            send(client.sockfd, message.c_str(), message.length(), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    std::string username;

    send(client_sock, " Enter your name: ", 18, 0);
    int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        close(client_sock);
        return;
    }

    username = std::string(buffer, bytes_received);
    {
        pthread_mutex_lock(&clients_mutex);
        clients.push_back(Client{client_sock, username});
        pthread_mutex_unlock(&clients_mutex);
    }

    std::string welcome_message = "Welcome, " + username + "!";
    send(client_sock, welcome_message.c_str(), welcome_message.length(), 0);
    broadcast_message(username + " connect to the chat.\n", client_sock);

    while (true) {
        bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        std::string message = std::string(buffer);

        if (message == "/exit") {
            break;
        } else if (message == "/list") {
            std::string client_list = "connected:\n";
            pthread_mutex_lock(&clients_mutex);
            for (const auto& client : clients) {
                client_list += client.username + "\n";
            }
            pthread_mutex_unlock(&clients_mutex);
            send(client_sock, client_list.c_str(), client_list.length(), 0);
        } else {
            broadcast_message(username + ": " + message, client_sock);
        }
    }

    {
        pthread_mutex_lock(&clients_mutex);
        auto it = std::remove_if(clients.begin(), clients.end(), [client_sock](const Client& client) {
            return client.sockfd == client_sock;
        });
        clients.erase(it, clients.end());
        pthread_mutex_unlock(&clients_mutex);
    }

    std::string goodbye_message = username + " left the chat.";
    broadcast_message(goodbye_message, client_sock);
    close(client_sock);
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        std::cerr << "Socket creating error." << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error." << std::endl;
        close(server_sock);
        return -1;
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        std::cerr << "Listening error." << std::endl;
        close(server_sock);
        return -1;
    }

    std::cout << "Server is on. Waiting for the clients..." << std::endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) {
            std::cerr << "Accepting error." << std::endl;
            continue;
        }
        std::cout << "Client is connected." << std::end
        std::thread(handle_client, client_sock).detach();
    }

    close(server_sock);
    return 0;
}
