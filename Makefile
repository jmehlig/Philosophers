# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 12:54:14 by jmehlig           #+#    #+#              #
#    Updated: 2022/05/04 14:32:46 by jmehlig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS_PATH = ./srcs/

SRCS =	$(SRCS_PATH)philo.c \
		$(SRCS_PATH)utils.c \
		$(SRCS_PATH)actions.c \

OBJS = $(SRCS:.c=.o)

CC		=		gcc

CFLAGS	=		-Wall -Wextra -Werror

all:	$(NAME)

.PHONY:	clean fclean re bonus bench bclean

$(NAME):
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(OBJS)

clean:
		rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<  -o $(<:.c=.o)
