/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/18 15:16:50 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

bool isShutdown = false;

Server::Server()
{
    serverSocket = -1;
    port = -1;
    epollSocket = -1;
    maxEvents = 9;
    events.resize(9);
    numEvents = 0;
    bytesReceived = 0;
    clientIndex = -1;
    clients.resize(9);
}

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

void Server::server(int ac, char **av)
{
    // Initialize the server
    serverInit(ac, av);

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
                    throw std::runtime_error("Epoll control client failed");
            }
            // If the event is for a client socket, receive and process the message
            else
            {
                std::fill(buffer, buffer + sizeof(buffer), 0);
                bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                std::cout << "Client " <<  events[i].data.fd << " sent: [" << buffer << "]" << std::endl;
                if (bytesReceived == 0)
                {
                    std::cout << "Client disconnected: " << events[i].data.fd << std::endl;
                    epoll_ctl(epollSocket, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
                    {
                        if (it->getClientSocket() == events[i].data.fd)
                        {
                            it->setClientSocket(-1);
                            it->setState(PASSWORD_REQUIRED);
                            it->setNickName("");
                            it->setUserName("");
                            it->setRealName("");
                            it->setIsRegistered(false);
                            it->setIsOperator(false);
                        }
                    }
                }
                else if (bytesReceived < 0)
                    throw std::runtime_error("Receive failed");
                // Authentication process
                else if (clients[clientIndex].getIsRegistered() == false)
                {
                    std::string message(buffer);

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
                else if (clients[clientIndex].getIsOperator() == true)
                {
                    std::string message(buffer);
                    trim(message);
                    std::istringstream iss(message);
                    std::string command;
                    iss >> command;

                    if (command == "TOPIC")
                        topicCommand(message, iss);
                    else if (command == "KICK")
                        kickCommand(message, iss);
                    else if (command == "INVITE")
                        inviteCommand(message, iss);
                    else if (command == "MODE")
                        modeCommand(message, iss);
                    
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

                        if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
                            return;
                        else if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
                        {
                            std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
                            std::string errorMsg = "Invalid channel name\n";
                            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                        }
                        else
                        {
                            // Create a new channel if it does not exist
                            std::map<std::string, Channel>::iterator it = channels.find(channelName);
                            if (it == channels.end())
                            {
                                Channel newChannel;
                                std::map<int, std::string> users;

                                users.insert(std::pair<int, std::string>(clients[clientIndex].getClientSocket(), clients[clientIndex].getNickName()));
                                newChannel.setChannelName(channelName);
                                newChannel.setUsers(users);
                                
                                it = channels.insert(std::pair<std::string, Channel>(channelName, newChannel)).first;

                                clients[clientIndex].setIsOperator(true);
                                
                                std::cout << "Client " << clients[clientIndex].getNickName() << " created channel: " << channelName << std::endl;
                                
                                std::string joinChannel = JOIN_CHANNEL(clients[clientIndex].getNickName(), channelName);
                                std::string noTopic = RPL_NOTOPIC(channelName);
                                std::string nameReply = RPL_NAMREPLY(it->second, clients[clientIndex].getNickName());
                                std::string endOfNames = RPL_ENDOFNAMES(channelName, clients[clientIndex].getNickName());

                                send(clients[clientIndex].getClientSocket(), joinChannel.c_str(), joinChannel.size(), 0);
                                send(clients[clientIndex].getClientSocket(), noTopic.c_str(), noTopic.size(), 0);
                                send(clients[clientIndex].getClientSocket(), nameReply.c_str(), nameReply.size(), 0);
                                send(clients[clientIndex].getClientSocket(), endOfNames.c_str(), endOfNames.size(), 0);
                            }
                            // Insert the client socket into the channel if it exists
                            else if (it->first == channelName)
                            {
                                std::map<int, std::string>& user = it->second.getUsers();
                                user.insert(std::pair<int, std::string>(clients[clientIndex].getClientSocket(), clients[clientIndex].getNickName()));

                                std::cout << "Client " << clients[clientIndex].getNickName() << " joined channel: " << channelName << std::endl;

                                std::string joinChannel = JOIN_CHANNEL(clients[clientIndex].getNickName(), channelName);
                                send(clients[clientIndex].getClientSocket(), joinChannel.c_str(), joinChannel.size(), 0);
                                
                                if (it->second.getTopic().empty())
                                {
                                    std::string noTopic = RPL_NOTOPIC(channelName);
                                    send(clients[clientIndex].getClientSocket(), noTopic.c_str(), noTopic.size(), 0);
                                }
                                else
                                {
                                    std::string checkTopic = RPL_TOPIC(channelName, it->second.getTopic());
                                    send(clients[clientIndex].getClientSocket(), checkTopic.c_str(), checkTopic.size(), 0);
                                }
                                
                                std::string nameReply = RPL_NAMREPLY(it->second, clients[clientIndex].getNickName());
                                send(clients[clientIndex].getClientSocket(), nameReply.c_str(), nameReply.size(), 0);
                                
                                std::string endOfNames = RPL_ENDOFNAMES(channelName, clients[clientIndex].getNickName());
                                send(clients[clientIndex].getClientSocket(), endOfNames.c_str(), endOfNames.size(), 0);

                            }
                        }
                    }
                }
            }
        }
    }
}

int Server::getServerSocket() const { return serverSocket; }

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
