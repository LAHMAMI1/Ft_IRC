/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:08:32 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/22 17:28:35 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::privmsgCommand(std::string& message, std::istringstream& iss)
{
    std::string target;
    std::string msg;

    iss >> target;
    std::getline(iss, msg);
    trim(msg);
    
    if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 2))
        return;
    if (target.empty())
    {
        std::string errorMsg = ERR_NORECIPIENT(clients[clientIndex].getNickName(), "PRIVMSG");
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    if (msg.empty())
    {
        std::string errorMsg = ERR_NOTEXTTOSEND(clients[clientIndex].getNickName());
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    if (msg[0] != ':')
    {
        std::string errorMsg = "Usage: /PRIVMSG <target> :<message>\n";
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (target[0] == '#')
    {
        std::map<std::string, Channel>::iterator it = channels.find(target);

        if (it == channels.end())
        {
            std::string errorMsg = ERR_NOSUCHCHANNEL(target);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        
        if (it->second.getUsers().find(clients[clientIndex].getClientSocket()) == it->second.getUsers().end())
        {
            std::string errorMsg = ERR_CANNOTSENDTOCHAN(clients[clientIndex].getNickName(), target);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }

        std::map<int, std::string>& users = it->second.getUsers();
        for (std::map<int, std::string>::iterator it = users.begin(); it != users.end(); it++)
        {
            if (it->first != clients[clientIndex].getClientSocket())
            {
                std::string msgToSend = ":" + clients[clientIndex].getNickName() + " PRIVMSG " + target + " " + msg + "\r\n";
                send(it->first, msgToSend.c_str(), msgToSend.size(), 0);
            }
        }
    }
    else
    {
        if (!findNickName(clients, target))
        {
            std::string errorMsg = ERR_NOSUCHNICK(target);
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->getNickName() == target)
            {
                std::string msgToSend = ":" + clients[clientIndex].getNickName() + " PRIVMSG " + target + " " + msg + "\r\n";
                send(it->getClientSocket(), msgToSend.c_str(), msgToSend.size(), 0);
            }
        }
    }
    
}