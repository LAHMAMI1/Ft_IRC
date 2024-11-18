/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:21:37 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/18 12:37:31 by olahmami         ###   ########.fr       */
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

// Create a function that will return the current time in the format: the day of the week, the month, the day of the month, the time, and the year.
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

void sendWelcomeMessages(int clientSocket, const std::string& nickName)
{
    send(clientSocket, RPL_WELCOME(nickName).c_str(), RPL_WELCOME(nickName).length(), 0);
    send(clientSocket, RPL_YOURHOST(nickName).c_str(), RPL_YOURHOST(nickName).length(), 0);
    send(clientSocket, RPL_CREATED(nickName).c_str(), RPL_CREATED(nickName).length(), 0);
    send(clientSocket, RPL_MYINFO(nickName).c_str(), RPL_MYINFO(nickName).length(), 0);
}

std::string intToString(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

bool ERR_NICKNAMEINUSE(const std::string& receivedNick, std::vector<Client>& clients, int clientSocket)
{
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->getNickName() == receivedNick)
        {
            std::string errorMsg = "Nickname already in use\n";
            send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
            return true;
        }
    }
    return false;
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