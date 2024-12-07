/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:35:17 by olahmami          #+#    #+#             */
/*   Updated: 2024/12/07 18:49:33 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

bool isValidUser(std::vector<std::string>& split_user)
{
    for (std::vector<std::string>::size_type i = 0; i < split_user.size(); i++)
        std::cout << "split_user[" << i << "] = " << split_user[i] << std::endl;
    if (!isAlphaWord(split_user[0]) || !isAlphaWord(split_user[3]))
        return false;
    if (split_user[1][0] != '0' || split_user[1].size() > 1)
        return false;
    if (split_user[2][0] != '*' || split_user[2].size() > 1)
        return false;
    if (split_user.size() == 5 && !isAlphaWord(split_user[4]))
        return false;
    return true;
}

void sendWelcomeMessages(int clientSocket, const std::string& nickName)
{
    send(clientSocket, RPL_WELCOME(nickName).c_str(), RPL_WELCOME(nickName).length(), 0);
    send(clientSocket, RPL_YOURHOST(nickName).c_str(), RPL_YOURHOST(nickName).length(), 0);
    send(clientSocket, RPL_CREATED(nickName).c_str(), RPL_CREATED(nickName).length(), 0);
    send(clientSocket, RPL_MYINFO(nickName).c_str(), RPL_MYINFO(nickName).length(), 0);
}

void Server::userCommand(std::string& message)
{
    std::string receivedUser;

    if (message.rfind("USER", 0) == 0)
    {
        if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 5))
            return;

        receivedUser = message.substr(5);
        trim(receivedUser);
        std::istringstream iss(receivedUser);
        std::vector<std::string> split_user;
        std::string word;

        while (iss >> word)
            split_user.push_back(word);

        if (!isValidUser(split_user))
        {
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " " << receivedUser << " :Invalid user" << std::endl;
            std::string errorMsg = "Invalid user\n";
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        }
        else
        {
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
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
}