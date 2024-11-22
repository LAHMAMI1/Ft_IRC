/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:59:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/22 18:32:36 by olahmami         ###   ########.fr       */
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

int Server::getServerSocket() const { return serverSocket; }

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
                getClientToServer(i);
            // If the event is for a client socket, receive and process the message
            else
            {
                std::fill(buffer, buffer + sizeof(buffer), 0);
                bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                std::cout << "Client " <<  events[i].data.fd << " sent: [" << buffer << "]" << std::endl;
            
                std::string message(buffer);
                trim(message);
                std::istringstream iss(message);
                std::string command;
                iss >> command;
                
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
                            it->setIsInvited(false);
                        }
                    }
                }
                else if (bytesReceived < 0)
                    throw std::runtime_error("Receive failed");
                // Authentication process
                else if (clients[clientIndex].getIsRegistered() == false)
                    Authentication(message);
                else if ((command == "PASS" || command == "NICK" || command == "USER") && clients[clientIndex].getIsRegistered() == true)
                {
                    std::string errorMsg = ERR_ALREADYREGISTERED(intToString(clients[clientIndex].getClientSocket()));
                    send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                }
                // Channels process
                else if (command == "JOIN")
                    joinCommand(message, iss);
                else if (command == "INVITE")
                    inviteCommand(message, iss);
                else if (command == "TOPIC")
                    topicCommand(message, iss);
                else if (command == "KICK")
                    kickCommand(message, iss);
                else if (command == "MODE")
                    modeCommand(message, iss);
                else if (command == "PRIVMSG")
                    privmsgCommand(message, iss);
                else if (command == "PING")
                    pingCommand(iss);
                else if (command == "QUIT")
                    quitCommand(iss);
            }
        }
    }
}
