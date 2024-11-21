/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:24:34 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 18:27:33 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::inviteCommand(std::string& message, std::istringstream& iss)
{
    if (clients[clientIndex].getIsOperator() == false)
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(clients[clientIndex].getNickName(), "INVITE");
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string targetNick;
    std::string channelName;
    
    iss >> targetNick;
    iss >> channelName;
    
    std::map<std::string, Channel>::iterator itChannel = channels.find(channelName);
    
    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
        return;
    if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
        std::string errorMsg = "Invalid channel name\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (itChannel == channels.end())
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(channelName);
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }

    std::map<int, std::string>& Users = itChannel->second.getUsers();
    if (!findNickName(clients, targetNick))
    {
        std::string errorMsg = ERR_NOSUCHNICK(targetNick);
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (clients[clientIndex].getNickName() == targetNick)
    {
        std::string errorMsg = "You cannot invite yourself\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (findNickInChannel(Users, targetNick))
    {
        std::string errorMsg = ERR_USERONCHANNEL(targetNick, channelName);
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else
    {
        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->getNickName() == targetNick)
            {
                it->setIsInvited(true);
                std::string inviteMsg = RPL_INVITING(clients[clientIndex].getNickName(), targetNick, channelName);
                send(it->getClientSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
                send(clients[clientIndex].getClientSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
                
                std::cout << "Client " << clients[clientIndex].getNickName() << " invited " << targetNick << " to " << channelName << std::endl;
                break;
            }
        }
    }
}