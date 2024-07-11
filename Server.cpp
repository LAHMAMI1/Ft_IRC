/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/11 14:08:02 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

bool isShutdown = false;

void Server::server(int ac, char **av)
{
     // Check if the correct number of arguments is provided
    if (ac != 3)
        throw std::invalid_argument("Usage: ./ircserv <port> <password>");

    // Set the password
    pwd = av[2];

    Client clients;
    // Create a socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        throw std::runtime_error("Socket creation failed");

    // Set the socket to non-blocking mode
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    // Set up the server address
    serverAddress.sin_family = AF_INET;
    port = (u_int16_t)atoi(av[1]);
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        throw std::runtime_error("Bind failed");

    // Start listening for incoming connections
    if (listen(serverSocket, 5) < 0)
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

    // Set the maximum number of events to be returned by epoll_wait
    maxEvents = 9;
    events.resize(maxEvents);
    while (isShutdown == false) 
    {
        // Wait for events to occur
        numEvents = epoll_wait(epollSocket, events.data(), maxEvents, -1);
        if (numEvents < 0 && isShutdown == false)
            throw std::runtime_error("Epoll wait failed");

        // Resize the events vector if it is full
        if (numEvents == maxEvents)
            events.resize(maxEvents + 1);

        for (int i = 0; i < numEvents; i++)
        {
            // If the event is for the server socket, accept the incoming connection
            if (events[i].data.fd == serverSocket)
            {
                int clientSocket = accept(serverSocket, NULL, NULL);
                if (clientSocket < 0)
                    throw std::runtime_error("Accept failed");
                clients.setClientSocket(clientSocket);
                
                std::string passwordRequest = "SERVER PASSWORD:\n";
                send(clientSocket, passwordRequest.c_str(), passwordRequest.size(), 0);

                std::cout << "Client connected: " << clientSocket << std::endl;

                // Add the client socket to the epoll instance
                clients.setClientEvents();
                if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, clients.getClientSocket(), &clients.getClientEvents()) < 0)
                {
                    throw std::runtime_error("Epoll control client failed");
                    close(clients.getClientSocket());
                }
            }
            // If the event is for a client socket, receive and process the message
            else
            {
                bzero(buffer, sizeof(buffer));
                bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    std::cout << "Client disconnected." << std::endl;
                    close(events[i].data.fd);
                }
                else
                {
                    std::string message(buffer);
                    // Check if the message starts with "PASS"
                    if (message.rfind("PASS", 0) == 0)
                    {
                        std::string receivedPassword = message.substr(5);
                        trim(receivedPassword);
                        if (receivedPassword == pwd)
                            std::cout << "Password verified." << std::endl;
                        else
                        {
                            std::cout << "Invalid password." << std::endl;
                            std::string errorMsg = "Invalid password.\n";
                            send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                            close(events[i].data.fd);
                        }
                    }
                    else
                        std::cout << "Message from client: " << message << std::endl;
                }
            }
        }
    }

    close(serverSocket);
}