/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:38:13 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/19 18:15:08 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/ircserv.hpp"

Client::Client()
{
    clientSocket = -1;
    state = PASSWORD_REQUIRED;
    nickName = "";
    userName = "";
    realName = "";
    isRegistered = false;
    isOperator = false;
    isInvited = false;
}

void Client::setClientSocket(int const& clientSocket) { this->clientSocket = clientSocket; }

int Client::getClientSocket() const { return clientSocket; }

void Client::setState(ClientState state) { this->state = state; }

ClientState Client::getState() const { return state; }

void Client::setNickName(std::string const& nickName) { this->nickName = nickName; }

std::string Client::getNickName() const { return nickName; }

void Client::setUserName(std::string const& userName) { this->userName = userName; }

std::string Client::getUserName() const { return userName; }

void Client::setRealName(std::string const& realName) { this->realName = realName; }

std::string Client::getRealName() const { return realName; }

void Client::setIsRegistered(bool const& isRegistered) { this->isRegistered = isRegistered; }

bool Client::getIsRegistered() const { return isRegistered; }

void Client::setIsOperator(bool const& isOperator) { this->isOperator = isOperator; }

bool Client::getIsOperator() const { return isOperator; }

void Client::setIsInvited(bool const& isInvited) { this->isInvited = isInvited; }

bool Client::getIsInvited() const { return isInvited; }