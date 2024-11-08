/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:39:21 by olahmami          #+#    #+#             */
/*   Updated: 2024/11/08 18:27:25 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Channel
{
    private:
        std::string channelName;
        std::map<int, std::string> users;

    public:
        // Setters and Getters
        std::string getChannelName() const;
        void setChannelName(std::string channelName);
        std::map<int, std::string>& getUsers();
        void setUsers(std::map<int, std::string> users);
};