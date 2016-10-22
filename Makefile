# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2016/10/22 13:24:41 by snicolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang
FLAGS=-Werror -Wextra -Wall
DRAW=libdraw
LIBFT=libft
LINKER=-L$(DRAW) -lm -ldraw
INC=-I$(DRAW)/headers -I $(LIBFT)/ -I./SOIL2-clone/SOIL2/ -Iheaders
SOIL=./SOIL2-clone/libSOIL2.a
ifeq ($(OS),Darwin)
	INC+=-I ~/.brew/include -I/usr/local/include
	LINKER+=-framework OpenGL -L ~/.brew/lib/ -L./SOIL2-clone/ -lSOIL2 -framework CoreFoundation -L/usr/local/lib -lglfw3 -L$(LIBFT) -lft
else
	LINKER+=-L./SOIL2-clone -lglfw -lGL -lSOIL2 -L$(LIBFT) -lft
endif
NAME=ogl
SRC=main.c loadobj.c events.c display.c
SRC_DIR=srcs
OBJ=$(SRC:%.c=$(BUILDDIR)/%.o)
BUILDDIR=build

all: $(NAME)

$(BUILDDIR):
	mkdir -p $@

$(NAME): $(SOIL) $(DRAW)/libdraw.a $(BUILDDIR) $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LINKER)

$(BUILDDIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(INC) $(FLAGS) $< -o $@

clean:
	$(RM) -r $(BUILDDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(LIBFT)/libft.a:
	make -j -C $(LIBFT)

$(DRAW)/libdraw.a: $(LIBFT)/libft.a
	make -j -C $(DRAW)

./SOIL2-clone/Makefile:
	git submodule init
	git submodule update

$(SOIL): ./SOIL2-clone/Makefile
	make -C ./SOIL2-clone/
