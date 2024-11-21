/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:21:58 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 18:27:18 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::modeCommand(std::string& message, std::istringstream& iss)
{
    if (clients[clientIndex].getIsOperator() == false)
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(clients[clientIndex].getNickName(), "MODE");
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string channelName;
    std::string mode;
    std::string param;

    iss >> channelName;
    iss >> mode;
    std::getline(iss, param);
    trim(param);

    std::map<std::string, Channel>::iterator it = channels.find(channelName);

    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 3))
        return;
    else if (mode.empty())
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