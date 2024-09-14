/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:06:30 by olahmami          #+#    #+#             */
/*   Updated: 2024/09/14 11:42:31 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Client
{
    private:
        int clientSocket;

    public:
        void setClientSocket(int const& clientSocket);
        int getClientSocket() const;
};