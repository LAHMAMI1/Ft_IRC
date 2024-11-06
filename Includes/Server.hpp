/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 09:55:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/06 17:49:48 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"
#include "Client.hpp"

class Server
{
    private:
        int serverSocket;
        sockaddr_in serverAddress;
        int port;
        int epollSocket;
        struct epoll_event evServer;
        int maxEvents;
        std::vector<struct epoll_event> events;
        int numEvents;
        char buffer[1024];
        int bytesReceived;
        std::string pwd;
        int clientIndex;
        std::vector<Client> clients;

    public:
        // Constructors
        Server();

        // methods
        void server(int ac, char **av);
        void serverInit(int ac, char **av);
        void closeAllSockets();

        // Setters and Getters
        int getServerSocket() const;

        // Command functions
        void passCommand(std::string message, int clientIndex, std::vector<Client>& clients);
        void nickCommand(std::string message, int clientIndex, std::vector<Client>& clients);
        void userCommand(std::string message, int clientIndex, std::vector<Client>& clients);
};