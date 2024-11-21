/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:35:17 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/21 15:35:36 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

void Server::userCommand(std::string& message)
{
    std::string receivedUser;

    if (message.rfind("USER", 0) == 0)
    {
        receivedUser = message.substr(5);
        trim(receivedUser);
        if (NEEDMOREPARAMS(message, clients[clientIndex].getClientSocket(), 5))
            return;
        else if (!isValidUser(receivedUser))
        {
            std::cout << "Client " << clients[clientIndex].getClientSocket() << " " << receivedUser << " :Invalid user" << std::endl;
            std::string errorMsg = "Invalid user\n";
            send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
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
        send(clients[clientIndex].getClientSocket(), errorMsg.c_str(), errorMsg.size(), 0);
    }
}