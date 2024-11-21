/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:38:04 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 18:36:02 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::joinCommand(std::string& message)
{
    std::string channelName = message.substr(5);
    trim(channelName);

    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
        return;
    else if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
        std::string errorMsg = "Invalid channel name\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else
    {
        // Create a new channel if it does not exist
        std::map<std::string, Channel>::iterator it = channels.find(channelName);
        if (it == channels.end())
        {
            Channel newChannel;
            std::map<int, std::string> users;

            users.insert(std::pair<int, std::string>(clients[clientIndex].getClientSocket(), clients[clientIndex].getNickName()));
            newChannel.setChannelName(channelName);
            newChannel.setUsers(users);
            
            it = channels.insert(std::pair<std::string, Channel>(channelName, newChannel)).first;

            clients[clientIndex].setIsOperator(true);
            
            std::cout << "Client " << clients[clientIndex].getNickName() << " created channel: " << channelName << std::endl;
            
            std::string joinChannel = JOIN_CHANNEL(clients[clientIndex].getNickName(), channelName);
            std::string noTopic = RPL_NOTOPIC(channelName);
            std::string nameReply = RPL_NAMREPLY(it->second, clients[clientIndex].getNickName());
            std::string endOfNames = RPL_ENDOFNAMES(channelName, clients[clientIndex].getNickName());

            send(clients[clientIndex].getClientSocket(), joinChannel.c_str(), joinChannel.size(), 0);
            send(clients[clientIndex].getClientSocket(), noTopic.c_str(), noTopic.size(), 0);
            send(clients[clientIndex].getClientSocket(), nameReply.c_str(), nameReply.size(), 0);
            send(clients[clientIndex].getClientSocket(), endOfNames.c_str(), endOfNames.size(), 0);
        }
        // Insert the client socket into the channel if it exists
        else if (it->first == channelName && (it->second.getInviteOnly() == false || clients[clientIndex].getIsInvited() == true))
        {
            
            std::map<int, std::string>& user = it->second.getUsers();
            user.insert(std::pair<int, std::string>(clients[clientIndex].getClientSocket(), clients[clientIndex].getNickName()));

            std::cout << "Client " << clients[clientIndex].getNickName() << " joined channel: " << channelName << std::endl;

            std::string joinChannel = JOIN_CHANNEL(clients[clientIndex].getNickName(), channelName);
            send(clients[clientIndex].getClientSocket(), joinChannel.c_str(), joinChannel.size(), 0);
            
            if (it->second.getTopic().empty())
            {
                std::string noTopic = RPL_NOTOPIC(channelName);
                send(clients[clientIndex].getClientSocket(), noTopic.c_str(), noTopic.size(), 0);
            }
            else
            {
                std::string checkTopic = RPL_TOPIC(channelName, it->second.getTopic());
                send(clients[clientIndex].getClientSocket(), checkTopic.c_str(), checkTopic.size(), 0);
            }
            
            std::string nameReply = RPL_NAMREPLY(it->second, clients[clientIndex].getNickName());
            send(clients[clientIndex].getClientSocket(), nameReply.c_str(), nameReply.size(), 0);
            
            std::string endOfNames = RPL_ENDOFNAMES(channelName, clients[clientIndex].getNickName());
            send(clients[clientIndex].getClientSocket(), endOfNames.c_str(), endOfNames.size(), 0);
        }
        else
        {
            std::string errorMsg = ERR_INVITEONLYCHAN(channelName);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
}