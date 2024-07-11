/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 09:55:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/10 15:58:39 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Server
{
    private:
        int serverSocket;
        sockaddr_in serverAddress;
        u_int16_t port;
        int epollSocket;
        struct epoll_event evServer;
        int maxEvents;
        std::vector<struct epoll_event> events;
        int numEvents;
        char buffer[1024];
        int bytesReceived;
        std::string pwd;

    public:
        void server(int ac, char **av);
};