/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:21:37 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/12 14:57:02 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

void    sigHandler(int signum)
{
    (void)signum;
    isShutdown = true;
}

std::string trim(std::string& trimmedStr)
{
    size_t start = trimmedStr.find_first_not_of(" \t\n\r\f\v");
    if (start != std::string::npos)
        trimmedStr = trimmedStr.substr(start);

    size_t end = trimmedStr.find_last_not_of(" \t\n\r\f\v");
    if (end != std::string::npos)
        trimmedStr = trimmedStr.substr(0, end + 1);

    return trimmedStr;
}

void closeIfNot(int fd)
{
    if (fd != -1)
        close(fd);
}

int ERR_NEEDMOREPARAMS(const std::string& message, int clientSocket)
{
    std::istringstream iss(message);
    std::vector<std::string> split_message;
    std::string word;

    while (iss >> word)
        split_message.push_back(word);
    
    if (split_message.size() < 2)
    {
        std::cout << clientSocket << " " << split_message[0] << " :Not enough parameters"<< std::endl;
        std::string errorMsg = "Not enough parameters\n";
        send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
        return 1;
    }
    return 0;
}