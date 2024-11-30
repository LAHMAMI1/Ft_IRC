# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/03 12:31:08 by olahmami          #+#    #+#              #
#    Updated: 2024/11/30 18:12:07 by olahmami         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -fsanitize=address
NAME = ircserv
SRCS = 	ircserv.cpp \
		srcs/Helpers/outils.cpp \
		srcs/Helpers/connection.cpp \
		srcs/Classes/Server.cpp \
		srcs/Classes/Client.cpp \
		srcs/Classes/Channel.cpp \
		srcs/Commands/INVITE.cpp \
		srcs/Commands/JOIN.cpp \
		srcs/Commands/KICK.cpp \
		srcs/Commands/MODE.cpp \
		srcs/Commands/NICK.cpp \
		srcs/Commands/PASS.cpp \
		srcs/Commands/TOPIC.cpp \
		srcs/Commands/USER.cpp \
		srcs/Commands/PRIVMSG.cpp \
		srcs/Commands/PING.cpp \
		srcs/Commands/QUIT.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re