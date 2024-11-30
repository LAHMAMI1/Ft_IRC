/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:30:49 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/30 18:10:28 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

void Server::passCommand(std::string& message)
{
    std::string receivedPassword;

    receivedPassword = message.substr(5);
    trim(receivedPassword);
    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
        return;
    else if (receivedPassword == pwd)
    {
        std::cout << "Client connected: " << clients[clientIndex].getClientSocket() << std::endl;
        std::string successMsg = "Password accepted. Welcome to the server.\n";
        send(clients[clientIndex].getClientSocket(), successMsg.c_str(), successMsg.size(), 0);
        clients[clientIndex].setState(NICK_REQUIRED);
    }
    else if (message.rfind("PASS", 0) != 0)
        return;
    else
    {
        std::string errorMsg = ERR_PASSWDMISMATCH(intToString(clients[clientIndex].getClientSocket()));
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.length(), 0);
    }
}