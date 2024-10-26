/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:06:30 by olahmami          #+#    #+#             */
/*   Updated: 2024/10/26 15:44:51 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

enum ClientState { PASSWORD_REQUIRED, NICK_REQUIRED, USER_REQUIRED };

class Client
{
    private:
        int clientSocket;
        ClientState state;
        std::string nickName;
        std::string userName;
        std::string realName;
        bool isRegistered;
        bool isOperator;

    public:
        // Constructors
        Client();

        // Setters and Getters
        void setClientSocket(int const& clientSocket);
        int getClientSocket() const;
        void setState(ClientState state);
        ClientState getState() const;
        void setNickName(std::string const& nickName);
        std::string getNickName() const;
        void setUserName(std::string const& userName);
        std::string getUserName() const;
        void setRealName(std::string const& realName);
        std::string getRealName() const;
        void setIsRegistered(bool const& isRegistered);
        bool getIsRegistered() const;
        void setIsOperator(bool const& isOperator);
        bool getIsOperator() const;
};