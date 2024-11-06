/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:32 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/06 17:51:41 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

int main(int ac, char **av)
{
    Server irc;

    signal(SIGINT, sigHandler); // Ctrl + C
    signal(SIGQUIT, sigHandler); // Ctrl + \|
    signal(SIGTSTP, sigHandler); // Ctrl + Z

    try
    {
        irc.server(ac, av);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        irc.closeAllSockets();
    }
    irc.closeAllSockets();
    std::cout << "Exiting IRC server" << std::endl;
    return 0;
}