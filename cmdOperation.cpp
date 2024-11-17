/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdOperation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:53:04 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/17 09:52:34 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Server::topicCommand(std::string& message, std::istringstream& iss)
{
    std::string channelName;
    std::string newTopic;

    iss >> channelName;
    std::getline(iss, newTopic);
    trim(newTopic);

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    
    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
        return;
    else if (channelName[0] != '#' || channelName.find(",") != std::string::npos)
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid channel name" << std::endl;
        std::string errorMsg = "Invalid channel name\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (newTopic.size() > 1 && newTopic[0] != ':')
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :Invalid topic format" << std::endl;
        std::string errorMsg = "Invalid topic format\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    else if (it == channels.end())
    {
        std::string errorMsg = "Channel does not exist\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
    // Clear the Topic
    else if (newTopic == ":")
    {
        if (it->second.getTopic().empty())
        {
            std::string noTopic = RPL_NOTOPIC(channelName);
            send(clients[clientIndex].getClientSocket(), noTopic.c_str(), noTopic.size(), 0);
        }
        else
        {
            it->second.setTopic("");
            std::string clearTopic = RPL_TOPIC(channelName, "");
            send(clients[clientIndex].getClientSocket(), clearTopic.c_str(), clearTopic.size(), 0);
        }
    }
    // Check the Topic
    else if (newTopic == "")
    {
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
    }
    // Set the Topic
    else
    {
        it->second.setTopic(newTopic.substr(1));
        std::string setTopic = RPL_TOPIC(channelName, newTopic.substr(1));
        send(clients[clientIndex].getClientSocket(), setTopic.c_str(), setTopic.size(), 0);
    }
}

void Server::kickCommand(std::string& message, std::istringstream& iss)
{
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