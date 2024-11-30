/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:28:23 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/30 18:10:42 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

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
    // Check the Topic
    if (newTopic == "")
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
    else if ((it->second.getProtectedTopic() == true && clients[clientIndex].getIsOperator() == true) 
        || it->second.getProtectedTopic() == false)
    {
        // Clear the Topic
        if (newTopic == ":")
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
                std::map<int, std::string>& user = it->second.getUsers();
                std::map<int, std::string>::iterator userIt;
                for (userIt = user.begin(); userIt != user.end(); ++userIt)
                {
                    if (userIt->first != clients[clientIndex].getClientSocket())
                        send(userIt->first, clearTopic.c_str(), clearTopic.size(), 0);
                }
            }
        }    
        // Set the Topic
        else if (!newTopic.empty())
        {
            it->second.setTopic(newTopic.substr(1));
            std::string setTopic = RPL_TOPIC(channelName, newTopic.substr(1));
            std::map<int, std::string>& user = it->second.getUsers();
            std::map<int, std::string>::iterator userIt;
            for (userIt = user.begin(); userIt != user.end(); ++userIt)
            {
                if (userIt->first != clients[clientIndex].getClientSocket())
                    send(userIt->first, setTopic.c_str(), setTopic.size(), 0);       
            }
        }
    }
    else
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(clients[clientIndex].getNickName(), channelName);
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
}
