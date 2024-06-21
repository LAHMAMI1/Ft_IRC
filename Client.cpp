/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:38:13 by olahmami          #+#    #+#             */
/*   Updated: 2024/06/21 11:37:27 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Client::setClientSocket(int const& clientSocket) { this->clientSocket = clientSocket; }

int Client::getClientSocket() const { return clientSocket; }

void Client::setClientEvents()
{
    evClient.events = EPOLLIN;
    evClient.data.fd = clientSocket;
}

struct epoll_event& Client::getClientEvents() { return evClient; }