#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./socketc <message> <port>" << std::endl;
        return 1;
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    u_int16_t port = (u_int16_t)atoi(av[2]);
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to server. Type messages to send. Type 'exit' to quit." << std::endl;

    while (true) {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        send(clientSocket, message.c_str(), message.length(), 0);
    }

    close(clientSocket);
    return 0;
}
