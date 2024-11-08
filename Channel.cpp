/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:48:38 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/08 18:27:05 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Channel.hpp"

std::string Channel::getChannelName() const { return channelName; }

void Channel::setChannelName(std::string channelName) { this->channelName = channelName; }

std::map<int, std::string>& Channel::getUsers()  { return users; }

void Channel::setUsers(std::map<int, std::string> users) { this->users = users; }