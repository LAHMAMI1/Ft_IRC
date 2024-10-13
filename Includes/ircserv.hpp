/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/13 12:08:17 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"

void    sigHandler(int signum);
std::string trim(std::string& trimmedStr);
void closeIfNot(int fd);
bool ERR_NEEDMOREPARAMS(const std::string& message, int clientSocket, long unsigned int numParams);
bool isValidNick(const std::string& receivedNick);
bool isAlphaWord(const std::string& word);
bool isValidUser(const std::string& receivedUser);