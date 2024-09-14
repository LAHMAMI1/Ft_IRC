/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:38:13 by olahmami          #+#    #+#             */
/*   Updated: 2024/09/14 11:42:41 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Client::setClientSocket(int const& clientSocket) { this->clientSocket = clientSocket; }

int Client::getClientSocket() const { return clientSocket; }
