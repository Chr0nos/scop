# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2016/10/21 16:52:24 by snicolet         ###   ########.fr        #
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
	LINKER+=-framework OpenGL -L ~/.brew/lib/ -L./SOIL2-clone/ -lSOIL2 -framework CoreFoundation -lglfw3
else
	LINKER+=-L./SOIL2-clone -lglfw3 -lGL -lSOIL2
endif
NAME=ogl
SRC=main.c
OBJ=$(SRC:%.c=$(BUILDDIR)/%.o)
BUILDDIR=build

all: $(NAME)

$(BUILDDIR):
	mkdir -p $@

$(NAME): $(SOIL) $(BUILDDIR) $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LINKER)

$(BUILDDIR)/%.o: %.c
	$(CC) -c $(INC) $(FLAGS) $< -o $@

clean:
	$(RM) -r $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

./SOIL2-clone/Makefile:
	git submodule init
	git submodule update

$(SOIL): ./SOIL2-clone/Makefile
	make -C ./SOIL2-clone/
