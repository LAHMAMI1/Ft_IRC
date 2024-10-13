/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:21:37 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/13 12:41:48 by olahmami         ###   ########.fr       */
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

bool ERR_NEEDMOREPARAMS(const std::string& message, int clientSocket, long unsigned int numParams)
{
    std::istringstream iss(message);
    std::vector<std::string> split_message;
    std::string word;

    while (iss >> word)
        split_message.push_back(word);
    
    if (split_message.size() < numParams)
    {
        std::cout << clientSocket << " " << split_message[0] << " :Not enough parameters"<< std::endl;
        std::ostringstream oss;
        oss << "Not enough parameters:\n";
        std::string errorMsg = oss.str();
        send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
        return true;
    }
    return false;
}

bool isValidNick(const std::string& receivedNick)
{
    if (!isalpha(receivedNick[0]))
        return false;
    for (size_t i = 1; i < receivedNick.size(); i++)
    {
        if (!isalnum(receivedNick[i]) && receivedNick[i] != '_' && receivedNick[i] != '-')
            return false;
    }
    return true;
}

bool isAlphaWord(const std::string& word)
{
    for (size_t i = 0; i < word.size(); i++)
    {
        if (!isalpha(word[i]))
            return false;
    }
    return true;
}

bool isValidUser(const std::string& receivedUser)
{
    std::istringstream iss(receivedUser);
    std::vector<std::string> split_user;
    std::string word;

    while (iss >> word)
        split_user.push_back(word);

    if (!isAlphaWord(split_user[0]) || !isAlphaWord(split_user[3]))
        return false;
    if (split_user[1][0] != '0' && split_user[1].size() > 1)
        return false;
    if (split_user[2][0] != '*' && split_user[2].size() > 1)
        return false;
    if (split_user.size() == 5 && !isAlphaWord(split_user[4]))
        return false;
    return true;
}