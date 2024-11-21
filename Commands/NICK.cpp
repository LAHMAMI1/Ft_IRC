/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:34:31 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 15:34:49 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::nickCommand(std::string& message)
{
    std::string receivedNick;

    if (message.rfind("NICK", 0) == 0)
    {
        receivedNick = message.substr(5);
        trim(receivedNick);
        if (ERR_NICKNAMEINUSE(receivedNick, clients, clients[clientIndex].getClientSocket()))
            return;
        if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
            return;
        else if (!isValidNick(receivedNick))
        {
            std::string errorMsg = ERR_ERRONEUSNICKNAME(intToString(clients[clientIndex].getClientSocket()), receivedNick);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
        else
        {
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " set nickname to: " << receivedNick << std::endl;
            std::string successMsg = "Nickname set to " + receivedNick + "\n";
            send(clients[clientIndex].getClientSocket(), successMsg.c_str(), successMsg.size(), 0);
            clients[clientIndex].setNickName(receivedNick);
            clients[clientIndex].setState(USER_REQUIRED);
        }
    }
    else
    {
        std::string errorMsg = ERR_NONICKNAMEGIVEN(intToString(clients[clientIndex].getClientSocket()));
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
}