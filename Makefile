# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adrean <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/13 13:24:19 by adrean            #+#    #+#              #
#    Updated: 2018/11/13 14:09:15 by adrean           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
HEADERS_DIR = include
SRC_DIR = src
OBJ_DIR = build

$(NAME):
	mkdir -p $(OBJ_DIR)
	gcc -Werror -Wextra -Wall -I$(HEADERS_DIR) -c $(SRC_DIR)/*
	mv *.o $(OBJ_DIR)
	ar rcs $(NAME) $(OBJ_DIR)/*

all: $(NAME)

clean:
	rm -f $(OBJ_DIR)/*

fclean: clean
	rm -f $(NAME)

re: fclean all

