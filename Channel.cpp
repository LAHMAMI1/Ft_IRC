/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:48:38 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/20 17:20:23 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Channel.hpp"

// Constructors
Channel::Channel()
{
    channelName = "";
    topic = "";
    inviteOnly = true;
    wichMode = "+i";
    protectedTopic = true;
    key = "";
    limit = 0;
}

// Setters and Getters
std::string Channel::getChannelName() const { return channelName; }

void Channel::setChannelName(std::string channelName) { this->channelName = channelName; }

std::map<int, std::string>& Channel::getUsers()  { return users; }

void Channel::setUsers(std::map<int, std::string> users) { this->users = users; }

std::string Channel::getTopic() const { return topic; }

void Channel::setTopic(std::string topic) { this->topic = topic; }

bool Channel::getInviteOnly() const { return inviteOnly; }

void Channel::setInviteOnly(bool inviteOnly) { this->inviteOnly = inviteOnly; }

std::string Channel::getWichMode() const { return wichMode; }

void Channel::setWichMode(std::string wichMode) { this->wichMode = wichMode; }

bool Channel::getProtectedTopic() const { return protectedTopic; }

void Channel::setProtectedTopic(bool protectedTopic) { this->protectedTopic = protectedTopic; }

std::string Channel::getKey() const { return key; }

void Channel::setKey(std::string key) { this->key = key; }

int Channel::getLimit() const { return limit; }

void Channel::setLimit(int limit) { this->limit = limit; }