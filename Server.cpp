/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/07 19:46:26 by olahmami         ###   ########.fr       */
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
    if (pwd.empty() || pwd.length() < 8)
        throw std::invalid_argument("Password must be at least 8 characters long and not empty");

    // Create a socket for the server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        throw std::runtime_error("Socket creation failed");

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

    // Set the maximum number of events to be returned by epoll_wait
    maxEvents = 9;
    events.resize(maxEvents);

    // Set the clients number
    std::vector<Client> clients(maxEvents);

    while (isShutdown == false) 
    {
        // Wait for events to occur
        numEvents = epoll_wait(epollSocket, events.data(), maxEvents, -1);
        if (numEvents < 0 && isShutdown == false)
            throw std::runtime_error("Epoll wait failed");

        // Resize the events vector if it is full
        if (numEvents == maxEvents)
            events.resize(maxEvents * 2);

        for (int i = 0; i < numEvents; i++)
        {
            // If the event is for the server socket, accept the incoming connection
            if (events[i].data.fd == serverSocket)
            {
                int clientSocket = accept(events[i].data.fd, NULL, NULL);
                if (clientSocket < 0)
                    throw std::runtime_error("Accept failed");

                // resize the clients vector if it is full
                if (clients.size() == clients.capacity())
                    clients.resize(clients.capacity() * 2);
                
                // Find the first available slot in the clients vector
                clientIndex = -1;
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
                if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
                    throw std::runtime_error("Failed to set non-blocking mode for the client socket");
                
                std::string passwordRequest = "ENTER SERVER PASSWORD:\n";
                send(clientSocket, passwordRequest.c_str(), passwordRequest.size(), 0);

                // Add the client socket to the epoll instance
                evServer.events = EPOLLIN | EPOLLET;
                evServer.data.fd = clients[clientIndex].getClientSocket();
                if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, clients[clientIndex].getClientSocket(), &evServer) < 0)
                {
                    throw std::runtime_error("Epoll control client failed");
                    closeIfNot(clients[clientIndex].getClientSocket());
                }
            }
            // If the event is for a client socket, receive and process the message
            else
            {
                std::fill(buffer, buffer + sizeof(buffer), 0);
                bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    std::cout << "Client disconnected: " << events[i].data.fd << std::endl;
                    epoll_ctl(epollSocket, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    closeIfNot(events[i].data.fd);
                }
                else
                {
                    std::string message(buffer);
                    std::string receivedPassword;
                    std::string receivedNick;

                    switch (clients[clientIndex].getState())
                    {
                        case PASSWORD_REQUIRED:
                            // If the message is not a PASS command, ignore it
                            if (message.rfind("PASS", 0) != 0)
                            {
                                std::cout << "Waiting for password from client: " << clients[clientIndex].getClientSocket() << std::endl;
                                continue;
                            }
                            
                            receivedPassword = message.substr(5);
                            trim(receivedPassword);
                            if (ERR_NEEDMOREPARAMS(message, events[i].data.fd))
                                continue;
                            else if (receivedPassword == pwd)
                            {
                                std::cout << "Client connected: " << clients[clientIndex].getClientSocket() << std::endl;
                                std::string successMsg = "Password accepted. Welcome to the server.\n";
                                send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
                                clients[clientIndex].setState(NICK_REQUIRED);
                            }
                            else
                            {
                                std::cout <<  clients[clientIndex].getClientSocket() << " :Password incorrect" << std::endl;
                                std::string errorMsg = "Password incorrect\n";
                                send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                            }
                            break;
                            
                        case NICK_REQUIRED:
                            if (message.rfind("NICK", 0) == 0)
                            {
                                receivedNick = message.substr(5);
                                trim(receivedNick);
                                if (ERR_NEEDMOREPARAMS(message, events[i].data.fd))
                                    continue;
                                else if (!isValidNick(receivedNick))
                                {
                                    std::cout << "Client " << clients[clientIndex].getClientSocket() << " " << receivedNick <<" :Invalid nickname" << std::endl;
                                    std::string errorMsg = "Invalid nickname\n";
                                    send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                                }
                                else
                                {
                                    std::cout << "Client " << clients[clientIndex].getClientSocket() << " set nickname to: " << receivedNick << std::endl;
                                    std::string successMsg = "Nickname set to " + receivedNick + "\n";
                                    send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
                                    clients[clientIndex].setState(USER_REQUIRED);
                                }
                            }
                            else
                            {
                                std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Nickname required" << std::endl;
                                std::string errorMsg = "Nickname required\n";
                                send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                            }
                            break;
                        // case USER_REQUIRED:
                        //     if (message.rfind("USER", 0) == 0)
                        //     {
                        //         std::string receivedUser = message.substr(5);
                        //         trim(receivedUser);
                        //         if (ERR_NEEDMOREPARAMS(message, events[i].data.fd))
                        //             continue;
                        //         else
                        //         {
                        //             std::cout << "Client " << clients.getClientSocket() << " set user to: " << receivedUser << std::endl;
                        //             std::string successMsg = "User set to " + receivedUser + "\n";
                        //             send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
                        //             state = READY;
                        //         }
                        //     }
                        //     else
                        //     {
                        //         std::cout << "Client " << clients.getClientSocket() << " :User required" << std::endl;
                        //         std::string errorMsg = "User required\n";
                        //         send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                        //     }
                        //     break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    closeIfNot(serverSocket);
    closeIfNot(epollSocket);
}

int Server::getServerSocket() const { return serverSocket; }