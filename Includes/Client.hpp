/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:06:30 by olahmami          #+#    #+#             */
/*   Updated: 2024/06/20 12:55:21 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Client
{
    private:
        int clientSocket;
        struct epoll_event evClient;

    public:
        void setClientSocket(int const& serverSocket);
        int getClientSocket() const;
        
        void setClientEvents(int const& epollSocket);
        struct epoll_event getClientEvents() const;
};