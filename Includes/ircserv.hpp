/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/18 12:38:16 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "macros.hpp"

void    sigHandler(int signum);
std::string trim(std::string& trimmedStr);
bool NEEDMOREPARAMS(const std::string& message, int clientSocket, long unsigned int numParams);
bool isValidNick(const std::string& receivedNick);
bool isAlphaWord(const std::string& word);
bool isValidUser(const std::string& receivedUser);
std::string getCurrentTime();
void sendWelcomeMessages(int clientSocket, const std::string& nickName);
std::string intToString(int num);
bool ERR_NICKNAMEINUSE(const std::string& receivedNick, std::vector<Client>& clients, int clientSocket);
std::string RPL_NAMREPLY(Channel channel, const std::string& nickName);
bool findNickName(const std::vector<Client>& clients, const std::string& target);
bool findNickInChannel(const std::map<int, std::string>& users, const std::string& target);