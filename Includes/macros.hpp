/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:59:58 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/10 12:15:34 by olahmami         ###   ########.fr       */
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

// Channel Messages
#define JOIN_CHANNEL(nickName, channelName) (":" + (nickName) + " JOIN " + (channelName) + "\r\n")
#define RPL_NOTOPIC(channelName) (":irc.chat.com 331 " + (channelName) + " :No topic is set\r\n")
#define RPL_ENDOFNAMES(channelName, nickName) (":irc.chat.com 366 " + (nickName) + " " + (channelName) + " :End of /NAMES list\r\n")
#define RPL_TOPIC(channelName, topic) (":irc.chat.com 332 " + (channelName) + " :" + (topic) + "\r\n")
// #define RPL_TOPICWHOTIME(channelName, nickName, topic) (":irc.chat.com 333 " + (channelName) + " " + (nickName) + " " + getCurrentTime() + " :" + (topic) + "\r\n")
// #define ERR_NOSUCHCHANNEL(channelName) (":irc.chat.com 403 " + (channelName) + " :No such channel\r\n")