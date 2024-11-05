/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/05 18:46:24 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

bool isShutdown = false;

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
}

void Server::server(int ac, char **av)
{
    // Initialize the server
    serverInit(ac, av);

    // Set the clients number
    std::vector<Client> clients(maxEvents);
    
    // Channel name -> set of client sockets
    std::map< std::string, std::set<int> > channels;

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
                // Authentication process
                else if (clients[clientIndex].getIsRegistered() == false)
                {
                    std::string message(buffer);
                    size_t newlinePos = message.find_first_of("\r\n");
                    if (newlinePos != std::string::npos)
                    {
                        std::string firstPart = message.substr(0, newlinePos);
                        std::string secondPart = message.substr(newlinePos + 2);
                        secondPart.erase(std::remove(secondPart.begin(), secondPart.end(), '\r'), secondPart.end());
                        secondPart.erase(std::remove(secondPart.begin(), secondPart.end(), '\n'), secondPart.end());
                        std::cout << "First part: [" << firstPart << "]" << std::endl;
                        std::cout << "Second part: [" << secondPart << "]" << std::endl;
                    }

                    switch (clients[clientIndex].getState())
                    {
                        case PASSWORD_REQUIRED:
                            passCommand(message, clientIndex, clients, events.data(), i);
                            break;

                        case NICK_REQUIRED:
                            nickCommand(message, clientIndex, clients, events.data(), i);
                            break;

                        case USER_REQUIRED:
                            userCommand(message, clientIndex, clients, events.data(), i);
                            break;
                    }
                }
                else
                {
                    std::string message(buffer);
                    if (message.rfind("PASS", 0) == 0 || message.rfind("NICK", 0) == 0 || message.rfind("USER", 0) == 0)
                    {
                        std::string errorMsg = ERR_ALREADYREGISTERED(intToString(clients[clientIndex].getClientSocket()));
                        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                    }
                    // Channels process
                    else if (message.rfind("JOIN", 0) == 0)
                    {
                        std::string channelName = message.substr(5);
                        trim(channelName);
                        if (NEEDMOREPARAMS(message, events[i].data.fd, 2))
                            return;
                        else if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
                        {
                            std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
                            std::string errorMsg = "Invalid channel name\n";
                            send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
                        }
                        else
                        {
                            // Create a new channel if it does not exist
                            if (channels.find(channelName) == channels.end())
                            {
                                std::set<int> clientSockets;
                                clients[clientIndex].setIsOperator(true);
                                clientSockets.insert(clients[clientIndex].getClientSocket());
                                channels.insert(std::pair< std::string, std::set<int> >(channelName, clientSockets));
                                std::cout << "Client " << clients[clientIndex].getNickName() << " created channel: " << channelName << std::endl;
                                std::cout << "Client " << clients[clientIndex].getNickName() << " joined channel: " << channelName << std::endl;
                                std::cout << "Client " << clients[clientIndex].getNickName() << " is operator in channel: " << channelName << std::endl;
                                std::string successMsg = "Channel " + channelName + " created and joined" + " as operator\n";
                                send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
                            }
                            // Insert the client socket into the channel if it exists
                            else
                            {
                                channels[channelName].insert(clients[clientIndex].getClientSocket());
                                std::cout << "Client " << clients[clientIndex].getNickName() << " joined channel: " << channelName << std::endl;
                                std::string successMsg = "Channel " + channelName + " joined\n";
                                send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
                            }
                        }
                    }
                }
            }
        }
    }
    closeIfNot(serverSocket);
    closeIfNot(epollSocket);
}

int Server::getServerSocket() const { return serverSocket; }