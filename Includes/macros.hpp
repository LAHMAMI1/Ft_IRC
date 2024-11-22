/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:59:58 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/22 17:59:45 by olahmami         ###   ########.fr       */
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
#define ERR_NOSUCHCHANNEL(channelName) (":irc.chat.com 403 " + (channelName) + " :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(nickName, channelName) (":irc.chat.com 441 " + (nickName) + " " + (channelName) + " :They aren't on that channel\r\n")
#define ERR_NOSUCHNICK(nickName) (":irc.chat.com 401 " + (nickName) + " :No such nick/channel\r\n")
#define ERR_USERONCHANNEL(nickName, channelName) (":irc.chat.com 443 " + (nickName) + " " + (channelName) + " :is already on channel\r\n")
#define ERR_INVITEONLYCHAN(channelName) (":irc.chat.com 473 " + (channelName) + " :Cannot join channel (+i)\r\n")
#define ERR_INVALIDMODEPARAM(nickName, channelName, mode, param) (":irc.chat.com 696 " + (nickName) + " " + (channelName) + " " + (mode) + " " + (param) + " :Invalid mode parameter\r\n")
#define ERR_INVALIDKEY(nickName, channelName) (":irc.chat.com 525 " + (nickName) + " " + (channelName) + " :Key is not well-formed\r\n")
#define ERR_CHANOPRIVSNEEDED(nickName, channelName) (":irc.chat.com 482 " + (nickName) + " " + (channelName) + " :You're not channel operator\r\n")
#define ERR_BADCHANNELKEY(nickName, channelName) (":irc.chat.com 475 " + (nickName) + " " + (channelName) + " :Cannot join channel (+k)\r\n")
#define ERR_CHANNELISFULL(nickName, channelName) (":irc.chat.com 471 " + (nickName) + " " + (channelName) + " :Cannot join channel (+l)\r\n")
#define ERR_NORECIPIENT(nickName, cmd) (":irc.chat.com 411 " + (nickName) + " :No recipient given (" + (cmd) + ")\r\n")
#define ERR_NOTEXTTOSEND(nickName) (":irc.chat.com 412 " + (nickName) + " :No text to send\r\n")
#define ERR_CANNOTSENDTOCHAN(nickName, channelName) (":irc.chat.com 404 " + (nickName) + " " + (channelName) + " :Cannot send to channel\r\n")
#define ERR_NOORIGIN(nickName) (":irc.chat.com 409 " + (nickName) + " :No origin specified\r\n")

// Channel Messages
#define JOIN_CHANNEL(nickName, channelName) (":" + (nickName) + " JOIN " + (channelName) + "\r\n")
#define RPL_NOTOPIC(channelName) (":irc.chat.com 331 " + (channelName) + " :No topic is set\r\n")
#define RPL_ENDOFNAMES(channelName, nickName) (":irc.chat.com 366 " + (nickName) + " " + (channelName) + " :End of /NAMES list\r\n")
#define RPL_TOPIC(channelName, topic) (":irc.chat.com 332 " + (channelName) + " :" + (topic) + "\r\n")
#define RPL_INVITING(nickName, targetNick, channelName) (":irc.chat.com 341 " + (nickName) + " " + (targetNick) + " " + (channelName) + "\r\n")
#define RPL_CHANNELMODEIS(channelName, mode) (":irc.chat.com 324 " + (channelName) + " " + (mode) + "\r\n")