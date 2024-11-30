/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:44:04 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/30 18:10:31 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

void Server::pingCommand(std::istringstream& iss)
{
    std::string serverName;
    iss >> serverName;
    
    if (serverName.empty())
    {
        std::string errorMsg = ERR_NOORIGIN(clients[clientIndex].getNickName());
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::cout << "PONG " << serverName << std::endl;
    std::string pongMsg = "PONG " + serverName + "\r\n";
    send(clients[clientIndex].getClientSocket(), pongMsg.c_str(), pongMsg.length(), 0);
}