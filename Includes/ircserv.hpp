/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:28 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/12 14:56:43 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"

void    sigHandler(int signum);
std::string trim(std::string& trimmedStr);
void closeIfNot(int fd);
int ERR_NEEDMOREPARAMS(const std::string& message, int clientSocket);