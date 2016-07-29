# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2016/07/28 18:16:18 by snicolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang
FLAGS=-Werror -Wextra -Wall
LINKER=-L../rt/libs/libdraw -lm -ldraw
INC=-I../rt/libs/libdraw/headers -I./SOIL2-clone/SOIL2/
SOIL=./SOIL2-clone/libSOIL2.a
ifeq ($(OS),Darwin)
	INC+=-I ~/.brew/include
	LINKER+=-framework OpenGL -L ~/.brew/lib/ -lglut -L./SOIL2-clone/ -lSOIL2 -framework CoreFoundation
else
	LINKER+=-L./SOIL2-clone -lglut -lGL -lSOIL2
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

./SOIL2-clone/Makefile:
	git submodule init
	git submodule update

$(SOIL):
	make -C ./SOIL2-clone/
