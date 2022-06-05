# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 12:54:14 by jmehlig           #+#    #+#              #
#    Updated: 2022/06/05 10:54:42 by jmehlig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

RM = rm -rf

AR = ar rs

SDIR = srcs
SRCS = 	philo.c \
		utils.c \
		actions.c \

ODIR = objs
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(NAME): $(addprefix $(SDIR)/, $(SRCS))
	$(CC) $(CFLAGS) -c $^
	@mkdir -p $(ODIR)
	@mv $(OBJS) $(ODIR)
	$(CC) $(CFLAGS) $(addprefix $(ODIR)/, $(OBJS)) -o $(NAME)
all: $(NAME)

clean:
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus .c .o
