/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:21:37 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/23 10:48:22 by olahmami         ###   ########.fr       */
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

bool NEEDMOREPARAMS(const std::string& message, int clientSocket, long unsigned int numParams)
{
    std::istringstream iss(message);
    std::vector<std::string> split_message;
    std::string word;

    while (iss >> word)
        split_message.push_back(word);
    
    if (split_message.size() < numParams)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(intToString(clientSocket), split_message[0]);
        send(clientSocket, errorMsg.c_str(), errorMsg.length(), 0);
        return true;
    }
    return false;
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

std::string getCurrentTime()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %b %d %T %Y", ltm);
    oss << buffer;
    return oss.str();
}

std::string intToString(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string RPL_NAMREPLY(Channel channel, const std::string& nickName)
{
    std::string NamesReply = ":irc_server 353 " + nickName + " = " + channel.getChannelName() + " :";
    for (std::map<int, std::string>::const_iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); ++it)
        NamesReply += it->second + " ";
    NamesReply += "\r\n";

    return NamesReply;
}

bool findNickName(const std::vector<Client>& clients, const std::string& target)
{
    for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->getNickName() == target)
            return true;
    }
    return false;
}

bool findNickInChannel(const std::map<int, std::string>& users, const std::string& target)
{
    for (std::map<int, std::string>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->second == target)
            return true;
    }
    return false;
}