#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 1024

double perform_operation(const std::string &operation, double num1, double num2) {
    if (operation == "ADD") {
        return num1 + num2;
    } else if (operation == "SUB") {
        return num1 - num2;
    } else if (operation == "MUL") {
        return num1 * num2;
    } else if (operation == "DIV") {
        if (num2 == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return 0;
        }
        return num1 / num2;
    } else {
        std::cerr << "Error: Unknown operation!" << std::endl;
        return 0;
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    while (true) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE] = {0};
        int read_size = read(client_socket, buffer, sizeof(buffer));
        if (read_size < 0) {
            perror("read failed");
            close(client_socket);
            continue;
        }

        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;

        size_t pos1 = command.find(' ');
        size_t pos2 = command.find(' ', pos1 + 1);

        std::string operation = command.substr(0, pos1);
        double num1 = std::stod(command.substr(pos1 + 1, pos2 - pos1 - 1));
        double num2 = std::stod(command.substr(pos2 + 1));

        double result = perform_operation(operation, num1, num2);

        std::string response = std::to_string(result);

        send(client_socket, response.c_str(), response.size(), 0);
        std::cout << "Result sent: " << response << std::endl;

        close(client_socket);
    }

    return 0;
}

