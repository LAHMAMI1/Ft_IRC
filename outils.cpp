/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:21:37 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/11 13:51:42 by olahmami         ###   ########.fr       */
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