/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdOperation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:53:04 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/20 18:12:37 by olahmami         ###   ########.fr       */
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
    // (void)message;
    std::string channelName;
    std::string mode;
    std::string param;

    iss >> channelName;
    iss >> mode;
    std::getline(iss, param);
    trim(param);

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
        else if (mode == "+t")
        {
            it->second.setProtectedTopic(true);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " to protected topic" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " to protected topic\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "-t")
        {
            it->second.setProtectedTopic(false);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " to not protected topic" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " to not protected topic\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "-k")
        {
            it->second.setKey("");
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " key to none" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " key to none\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "-l")
        {
            it->second.setLimit(0);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " to no limit" << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " to no limit\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else
        {
            std::string errorMsg = ERR_INVALIDMODEPARAM(clients[clientIndex].getNickName(), channelName, mode, param);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
    else
    {
        if (mode == "+k")
        {
            if (param.find(' '))
            {
                std::string errorMsg = ERR_INVALIDKEY(clients[clientIndex].getNickName(), channelName);
                send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                return;
            }
            it->second.setKey(param);
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " key to " << param << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " key to " + param + "\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "+l")
        {
            if (param.find_first_not_of("0123456789") != std::string::npos)
            {
                std::string errorMsg = ERR_INVALIDMODEPARAM(clients[clientIndex].getNickName(), channelName, mode, param);
                send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                return;
            }
            it->second.setLimit(std::atoi(param.c_str()));
            std::cout << "Client " << clients[clientIndex].getNickName() << " set channel " << channelName << " limit to " << param << std::endl;
            std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set channel " + channelName + " limit to " + param + "\n";
            send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
        }
        else if (mode == "+o")
        {
            std::map<int, std::string>& users = it->second.getUsers();
            bool userFound = false;

            for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
            {
                if (it->second == param)
                {
                    clients[it->first].setIsOperator(true);
                    std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " set " + param + " as operator\n";
                    send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
                    userFound = true;
                    break;
                }
            }
            if (!userFound)
            {
                std::string errorMsg = ERR_NOSUCHNICK(param);
                send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            }
        }
        else if (mode == "-o")
        {
            std::map<int, std::string>& users = it->second.getUsers();
            bool userFound = false;

            for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); ++it)
            {
                if (it->second == param)
                {
                    clients[it->first].setIsOperator(false);
                    std::string rplMsg = "Client " + clients[clientIndex].getNickName() + " removed " + param + " as operator\n";
                    send(clients[clientIndex].getClientSocket(), rplMsg.c_str(), rplMsg.size(), 0);
                    userFound = true;
                    break;
                }
            }
            if (!userFound)
            {
                std::string errorMsg = ERR_NOSUCHNICK(param);
                send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            }
        }
        else
        {
            std::string errorMsg = ERR_INVALIDMODEPARAM(clients[clientIndex].getNickName(), channelName, mode, param);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
    }
}