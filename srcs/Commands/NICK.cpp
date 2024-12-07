/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:34:31 by olahmami          #+#    #+#             */
/*   Updated: 2024/12/07 18:10:41 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

bool NICKNAMEINUSE(std::string& receivedNick, std::vector<Client>& clients, int clientSocket)
{
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->getNickName() == receivedNick)
        {
            std::string errorMsg = ERR_NICKNAMEINUSE(intToString(clientSocket), receivedNick);
            send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
            return true;
        }
    }
    return false;
}

bool isValidNick(std::string& receivedNick)
{
    if (!isalpha(receivedNick[0]))
        return false;
    for (size_t i = 1; i < receivedNick.size(); i++)
    {
        if (!isalnum(receivedNick[i]) && receivedNick[i] != '_' && receivedNick[i] != '-')
            return false;
    }
    return true;
}

void Server::nickCommand(std::string& message)
{
    std::string receivedNick;

    if (message.rfind("NICK", 0) == 0)
    {
        if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
            return;

        receivedNick = message.substr(5);
        trim(receivedNick);

        if (NICKNAMEINUSE(receivedNick, clients, clients[clientIndex].getClientSocket()))
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