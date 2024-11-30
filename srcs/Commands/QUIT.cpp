/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:04:09 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/30 18:10:38 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/ircserv.hpp"

void Server::quitCommand(std::istringstream& iss)
{
    std::string reason;
    std::getline(iss, reason);
    trim(reason);

    if (reason.empty())
        reason = "Client QUIT";
    
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if (findNickInChannel(it->second.getUsers(), clients[clientIndex].getNickName()))
        {
            it->second.getUsers().erase(clients[clientIndex].getClientSocket());
            std::cout << "Client " << clients[clientIndex].getNickName() << " left channel: " << it->first << std::endl;
        }
    }
}