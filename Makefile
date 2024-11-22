# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/03 12:31:08 by olahmami          #+#    #+#              #
#    Updated: 2024/11/22 18:03:07 by olahmami         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra -fsanitize=address
NAME = ircserv
SRCS = 	ircserv.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		outils.cpp \
		connection.cpp \
		Commands/INVITE.cpp \
		Commands/JOIN.cpp \
		Commands/KICK.cpp \
		Commands/MODE.cpp \
		Commands/NICK.cpp \
		Commands/PASS.cpp \
		Commands/TOPIC.cpp \
		Commands/USER.cpp \
		Commands/PRIVMSG.cpp \
		Commands/PING.cpp \
		Commands/QUIT.cpp \

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