/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/05 18:27:42 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "macros.hpp"

void    sigHandler(int signum);
std::string trim(std::string& trimmedStr);
void closeIfNot(int fd);
bool NEEDMOREPARAMS(const std::string& message, int clientSocket, long unsigned int numParams);
bool isValidNick(const std::string& receivedNick);
bool isAlphaWord(const std::string& word);
bool isValidUser(const std::string& receivedUser);
std::string getCurrentTime();
void sendWelcomeMessages(int clientSocket, const std::string& nickName);
std::string intToString(int num);
bool NICKNAMEINUSE(const std::string& receivedNick, std::vector<Client>& clients, int clientSocket);