/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:38:13 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/06 16:12:45 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

Client::Client() : clientSocket(-1), state(PASSWORD_REQUIRED) {}

void Client::setClientSocket(int const& clientSocket) { this->clientSocket = clientSocket; }

int Client::getClientSocket() const { return clientSocket; }

void Client::setState(ClientState state) { this->state = state; }

ClientState Client::getState() const { return state; }
