/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:44:22 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/24 12:27:30 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Server::serverInit(int ac, char **av)
{
    // Check if the correct number of arguments is provided
    if (ac != 3)
        throw std::invalid_argument("Usage: ./ircserv <port> <password>");

    // Set the password
    pwd = av[2];
    if (pwd.empty() || pwd.length() < 8)
        throw std::invalid_argument("Password must be at least 8 characters long and not empty");

    // Create a socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        throw std::runtime_error("Socket creation failed");

    // Set SO_REUSEADDR to allow immediate reuse of the port
    int optval = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("Failed to set SO_REUSEADDR on server socket");

    // Set the socket to non-blocking mode
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set non-blocking mode for the server socket");

    // Set up the server address
    serverAddress.sin_family = AF_INET;
    port = atoi(av[1]);
    if (port < 1024 || port > 65535)
        throw std::invalid_argument("Port must be a number between 1024 and 65535");
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        throw std::runtime_error("Bind failed");

    // Start listening for incoming connections
    if (listen(serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("Listen failed");

    // Create an epoll instance
    epollSocket = epoll_create1(0);
    if (epollSocket < 0)
        throw std::runtime_error("Epoll creation failed");

    // Add the server socket to the epoll instance
    evServer.events = EPOLLIN;
    evServer.data.fd = serverSocket;
    if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, serverSocket, &evServer) < 0)
        throw std::runtime_error("Epoll control server failed");

    std::cout << "IRC server is waiting for incoming connections" << std::endl;
}

void Server::getClientToServer(int& i)
{
    int clientSocket = accept(events[i].data.fd, NULL, NULL);
    if (clientSocket < 0)
        throw std::runtime_error("Accept failed");

    // resize the clients vector if it is full
    if (clients.size() == clients.capacity())
        clients.resize(clients.capacity() * 2);
    
    // Find the first available slot in the clients vector
    for (std::vector<Client>::size_type j = 0; j < clients.size(); j++)
    {
        if (clients[j].getClientSocket() == -1)
        {
            clientIndex = j;
            break;
        }
    }

    if (clientIndex == -1)
        throw std::runtime_error("No available slot for new client");

    clients[clientIndex].setClientSocket(clientSocket);

    // Set the client socket to non-blocking mode
    if (fcntl(clients[clientIndex].getClientSocket(), F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set non-blocking mode for the client socket");

    std::string passwordRequest = "ENTER SERVER PASSWORD:\n";
    send(clients[clientIndex].getClientSocket(), passwordRequest.c_str(), passwordRequest.size(), 0);

    // Add the client socket to the epoll instance
    evServer.events = EPOLLIN;
    evServer.data.fd = clients[clientIndex].getClientSocket();
    if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, clients[clientIndex].getClientSocket(), &evServer) < 0)
        throw std::runtime_error("Epoll control client failed");
}

void Server::Authentication(std::string& message)
{
    switch (clients[clientIndex].getState())
    {
        case PASSWORD_REQUIRED:
            passCommand(message);
            break;

        case NICK_REQUIRED:
            nickCommand(message);
            break;

        case USER_REQUIRED:
            userCommand(message);
            break;
    }
}

void Server::closeAllSockets()
{
    // Close all client sockets
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        int clientSocket = it->getClientSocket();
        if (clientSocket != -1)
        {
            std::cout << "\nClient disconnected: " << clientSocket;
            if (epoll_ctl(epollSocket, EPOLL_CTL_DEL, clientSocket, NULL) < 0)
                std::cerr << "Error removing client socket " << clientSocket << " from epoll\n";
            
            if (close(clientSocket) < 0)
                std::cerr << "Error closing client socket " << clientSocket << std::endl;

            it->setClientSocket(-1);
        }
    }

    // Close server socket
    std::cout << "\nClosing server socket: " << serverSocket << std::endl;
    if (epoll_ctl(epollSocket, EPOLL_CTL_DEL, serverSocket, NULL) < 0)
        std::cerr << "Error removing server socket from epoll\n";

    if (close(serverSocket) < 0)
        std::cerr << "Error closing server socket\n";

    // Close the epoll instance
    std::cout << "Closing epoll socket: " << epollSocket << std::endl;
    if (close(epollSocket) < 0)
        std::cerr << "Error closing epoll socket\n";

    std::cout << "All sockets closed.\n";
}