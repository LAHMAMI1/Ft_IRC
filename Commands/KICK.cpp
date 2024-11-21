/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:15:00 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 18:28:28 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::kickCommand(std::string& message, std::istringstream& iss)
{
    if (clients[clientIndex].getIsOperator() == false)
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(clients[clientIndex].getNickName(), "KICK");
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string channelName;
    std::string nickName;
    std::string comment;

    iss >> channelName;
    iss >> nickName;
    std::getline(iss, comment);
    trim(comment);

    if (comment.empty())
        comment = "FOR NO REASON";

    std::map<std::string, Channel>::iterator it = channels.find(channelName);

    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 3))
        return;
    else if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
        std::string errorMsg = "Invalid channel name\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (it == channels.end())
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(channelName);
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else
    {
        std::map<int, std::string>& users = it->second.getUsers();
        bool userFound = false;

        for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
        {
            if (it->second == nickName)
            {
                std::string kickMessage = clients[clientIndex].getNickName() + "KICK " + channelName + " " + nickName + " :" + comment + "\r\n";
                send(it->first, kickMessage.c_str(), kickMessage.size(), 0);
                users.erase(it);
                userFound = true;
                break;
            }
        }
        if (!userFound)
        {
            std::string errorMsg = ERR_USERNOTINCHANNEL(nickName, channelName);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
}