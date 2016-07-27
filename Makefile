# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#*   Updated: 2016/07/27 18:13:39 by snicolet         ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang
FLAGS=-Werror -Wextra -Wall
LINKER=-L../rt/libs/libdraw -lm -ldraw
INC=-I../rt/libs/libdraw/headers
ifeq ($(OS),Darwin)
	LINKER+=-framework OpenGL -lglut
else
	LINKER+=-lglut -lGL
endif
NAME=ogl
OBJ=main.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LINKER)

%.o: %.c
	$(CC) -c $(INC) $(FLAGS) $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
