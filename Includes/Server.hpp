/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 09:55:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/12/07 17:55:17 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
        std::map<std::string, Channel> channels;

    public:
        // Constructors
        Server();

        // methods
        void server(int ac, char **av);
        void serverInit(int ac, char **av);
        void closeAllSockets();
        void getClientToServer(int& i);
        void Authentication(std::string& message);

        // Setters and Getters
        int getServerSocket() const;

        // Command Authentication
        void passCommand(std::string& message);
        void nickCommand(std::string& message);
        void userCommand(std::string& message);

        // Join Command
        void joinCommand(std::string& message, std::istringstream& iss);

        // Command Operations
        void topicCommand(std::string& message, std::istringstream& iss);
        void kickCommand(std::string& message, std::istringstream& iss);
        void inviteCommand(std::string& message, std::istringstream& iss);
        void modeCommand(std::string& message, std::istringstream& iss);

        // Command Messages
        void privmsgCommand(std::string& message, std::istringstream& iss);
        void pingCommand(std::istringstream& iss);
        void quitCommand(std::istringstream& iss);
};