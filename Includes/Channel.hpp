/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:39:21 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/19 17:19:38 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Channel
{
    private:
        std::string channelName;
        std::map<int, std::string> users;
        std::string topic;
        bool inviteOnly; // +i
        std::string wichMode;

    public:
        // Constructors
        Channel();

        // Setters and Getters
        std::string getChannelName() const;
        void setChannelName(std::string channelName);
        std::map<int, std::string>& getUsers();
        void setUsers(std::map<int, std::string> users);
        std::string getTopic() const;
        void setTopic(std::string topic);
        bool getInviteOnly() const;
        void setInviteOnly(bool inviteOnly);
        std::string getWichMode() const;
        void setWichMode(std::string wichMode);
};