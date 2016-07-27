# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#*   Updated: 2016/07/27 20:35:37 by snicolet         ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang
FLAGS=-Werror -Wextra -Wall -Weverything -Wno-padded
LINKER=-L../rt/libs/libdraw -lm -ldraw
INC=-I../rt/libs/libdraw/headers
ifeq ($(OS),Darwin)
	INC+=-I./libSOIL/
	LINKER+=-framework OpenGL -lglut -L./libSOIL -lSOIL
else
	INC+=/usr/include/SOIL/
	LINKER+=-lglut -lGL -lSOIL
endif
NAME=ogl
OBJ=main.o
SOIL=./libSOIL/libSOIL.a

all: $(NAME)

$(NAME): $(SOIL) $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LINKER)

%.o: %.c
	$(CC) -c $(INC) $(FLAGS) $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(SOIL):
	make -C ./libSOIL/
