/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 09:55:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/05 12:34:37 by olahmami         ###   ########.fr       */
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

    public:
        // methods
        void server(int ac, char **av);
        void serverInit(int ac, char **av);
        
        // Setters and Getters
        int getServerSocket() const;

        // Command functions
        void passCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i);
        void nickCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i);
        void userCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i);
};