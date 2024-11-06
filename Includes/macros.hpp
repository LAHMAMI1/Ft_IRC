/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:59:58 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/06 14:33:37 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

// Welcome Messages
#define RPL_WELCOME(nickName) (":irc.chat.com 001 " + (nickName) + " :Welcome to the irchat Network, " + (nickName) + "\r\n")
#define RPL_YOURHOST(nickName) (":irc.chat.com 002 " + (nickName) + " :Your host is irchat, running version 1.0\r\n")
#define RPL_CREATED(nickName) (":irc.chat.com 003 " + (nickName) + " :This server was created " + getCurrentTime() + "\r\n")
#define RPL_MYINFO(nickName) (":irc.chat.com 004 " + (nickName) + " irchat 1.0 iotlk\r\n")

// Error Messages
#define ERR_NEEDMOREPARAMS(clientSocket, cmd) (":irc.chat.com 461 " + (clientSocket) + " " + (cmd) + " :Not enough parameters\r\n")
#define ERR_PASSWDMISMATCH(clientSocket) (":irc.chat.com 464 " + (clientSocket) + " :Password incorrect\r\n")
#define ERR_ALREADYREGISTERED(clientSocket) (":irc.chat.com 462 " + (clientSocket) + " :You may not reregister\r\n")
#define ERR_ERRONEUSNICKNAME(clientSocket, nick) (":irc.chat.com 432 " + (clientSocket) + " " + (nick) + " :Erroneus nickname\r\n")
#define ERR_NONICKNAMEGIVEN(clientSocket) (":irc.chat.com 431 " + (clientSocket) + " :No nickname given\r\n")

