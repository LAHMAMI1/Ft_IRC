/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:48:38 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/10 10:15:58 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Channel.hpp"

// Constructors
Channel::Channel()
{
    channelName = "";
    topic = "";
}

// Setters and Getters
std::string Channel::getChannelName() const { return channelName; }

void Channel::setChannelName(std::string channelName) { this->channelName = channelName; }

std::map<int, std::string>& Channel::getUsers()  { return users; }

void Channel::setUsers(std::map<int, std::string> users) { this->users = users; }

std::string Channel::getTopic() const { return topic; }

void Channel::setTopic(std::string topic) { this->topic = topic; }