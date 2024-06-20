/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:38:13 by olahmami          #+#    #+#             */
/*   Updated: 2024/06/20 16:02:29 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Client::setClientSocket(int const& serverSocket)
{
    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket < 0)
        throw std::runtime_error("Accept failed");
}

int Client::getClientSocket() const { return clientSocket; }

void Client::setClientEvents(int const& epollSocket)
{
    evClient.events = EPOLLIN;
    evClient.data.fd = clientSocket;
    if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, clientSocket, &evClient) < 0)
    {
        throw std::runtime_error("Epoll control client failed");
        close(clientSocket);
    }
}

struct epoll_event Client::getClientEvents() const { return evClient; }