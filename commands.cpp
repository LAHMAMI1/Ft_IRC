/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 11:37:17 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/05 18:28:05 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void Server::passCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i)
{
    std::string receivedPassword;

    if (message.rfind("PASS", 0) != 0)
    {
        std::cout << "Waiting for password from client: " << clients[clientIndex].getClientSocket() << std::endl;
        return;
    }

    receivedPassword = message.substr(5);
    trim(receivedPassword);
    if (NEEDMOREPARAMS(message, events[i].data.fd, 2))
        return;
    else if (receivedPassword == pwd)
    {
        std::cout << "Client connected: " << clients[clientIndex].getClientSocket() << std::endl;
        std::string successMsg = "Password accepted. Welcome to the server.\n";
        send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
        clients[clientIndex].setState(NICK_REQUIRED);
    }
    else
    {
        std::string errorMsg = ERR_PASSWDMISMATCH(intToString(events[i].data.fd));
        send(events[i].data.fd, errorMsg.c_str(), errorMsg.length(), 0);
    }
}

void Server::nickCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i)
{
    std::string receivedNick;

    if (message.rfind("NICK", 0) == 0)
    {
        receivedNick = message.substr(5);
        trim(receivedNick);
        if (NICKNAMEINUSE(receivedNick, clients, events[i].data.fd))
            return;
        if (NEEDMOREPARAMS(message, events[i].data.fd, 2))
            return;
        else if (!isValidNick(receivedNick))
        {
            std::string errorMsg = ERR_ERRONEUSNICKNAME(intToString(events[i].data.fd), receivedNick);
            send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
        }
        else
        {
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " set nickname to: " << receivedNick << std::endl;
            std::string successMsg = "Nickname set to " + receivedNick + "\n";
            send(events[i].data.fd, successMsg.c_str(), successMsg.size(), 0);
            clients[clientIndex].setNickName(receivedNick);
            clients[clientIndex].setState(USER_REQUIRED);
        }
    }
    else
    {
        std::string errorMsg = ERR_NONICKNAMEGIVEN(intToString(events[i].data.fd));
        send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
    }
}

void Server::userCommand(std::string message, int clientIndex, std::vector<Client>& clients, struct epoll_event* events, int i)
{
    std::string receivedUser;

    if (message.rfind("USER", 0) == 0)
    {
        receivedUser = message.substr(5);
        trim(receivedUser);
        if (NEEDMOREPARAMS(message, events[i].data.fd, 5))
            return;
        else if (!isValidUser(receivedUser))
        {
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " " << receivedUser << " :Invalid user" << std::endl;
            std::string errorMsg = "Invalid user\n";
            send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
        }
        else
        {
            std::istringstream iss(receivedUser);
            std::vector<std::string> split_user;
            std::string part;
            while (iss >> part)
                split_user.push_back(part);
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " set user to: " << split_user[0] << std::endl;
            sendWelcomeMessages(clients[clientIndex].getClientSocket(), clients[clientIndex].getNickName());
            clients[clientIndex].setUserName(split_user[0]);
            clients[clientIndex].setRealName(split_user[3]);
            clients[clientIndex].setIsRegistered(true);
        }
    }
    else
    {
        std::cout << "Client " << clients[clientIndex].getClientSocket() << " :User required" << std::endl;
        std::string errorMsg = "User required\n";
        send(events[i].data.fd, errorMsg.c_str(), errorMsg.size(), 0);
    }
}