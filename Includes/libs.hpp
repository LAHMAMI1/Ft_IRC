/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:18:15 by olahmami          #+#    #+#             */
/*   Updated: 2024/07/10 15:48:51 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <cstddef>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <csignal>
#include <algorithm>
#include <cctype>
#include <sstream>

extern bool isShutdown;