/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:06:30 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/06 16:50:42 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

enum ClientState { PASSWORD_REQUIRED, NICK_REQUIRED, USER_REQUIRED, READY };

class Client
{
    private:
        int clientSocket;
        ClientState state;

    public:
        // Constructors
        Client();

        // Setters and Getters
        void setClientSocket(int const& clientSocket);
        int getClientSocket() const;
        void setState(ClientState state);
        ClientState getState() const;
};