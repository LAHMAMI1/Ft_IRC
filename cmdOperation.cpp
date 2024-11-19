/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdOperation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:53:04 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/19 18:18:03 by olahmami         ###   ########.fr       */
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

void Server::inviteCommand(std::string& message, std::istringstream& iss)
{
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

void Server::modeCommand(std::string& message, std::istringstream& iss)
{
    std::string channelName;
    std::string mode;
    std::string param;

    iss >> channelName;
    iss >> mode;
    iss >> param;

    std::map<std::string, Channel>::iterator it = channels.find(channelName);

    if (mode.empty())
    {
        std::string rplMsg = RPL_CHANNELMODEIS(channelName, it->second.getWichMode());
        send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
    }
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

    if (param.empty())
    {
        if (mode == "+i")
        {
            it->second.setInviteOnly(true);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " to invite only" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " to invite only\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "-i")
        {
            it->second.setInviteOnly(false);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " to not invite only" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " to not invite only\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
    }
}