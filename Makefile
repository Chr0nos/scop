# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2016/07/27 21:28:37 by snicolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang
FLAGS=-Werror -Wextra -Wall -Weverything -Wno-padded
LINKER=-L../rt/libs/libdraw -lm -ldraw
INC=-I../rt/libs/libdraw/headers -I./SOIL2-clone/SOIL2/
ifeq ($(OS),Darwin)
	SOIL=./SOIL2-clone/libSOIL.a
	LINKER+=-framework OpenGL -lglut -L./SOIL2-clone/ -lSOIL2 -framework CoreFoundation
else
	LINKER+=-lglut -lGL -lSOIL
endif
NAME=ogl
OBJ=main.o

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

./SOIL2-clone:
	git submodule init
	git submodule update

$(SOIL):
	make -C ./SOIL2-clone/
