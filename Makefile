# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2017/05/11 18:44:25 by snicolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
CC=clang -O2 -march=native -mtune=native -g3 -fsanitize=address
FLAGS=-Werror -Wextra -Wall -Weverything -Wno-reserved-id-macro -Wno-documentation -Wno-documentation-unknown-command -Wno-padded
DRAW=libdraw
LIBFT=libft
LINKER=-L$(DRAW) -lm -ldraw -lGLEW
INC=-I$(DRAW)/headers -I $(LIBFT)/ -I./SOIL2-clone/incs -Iheaders
SOIL=./SOIL2-clone/libSOIL2.a
ifeq ($(OS),Darwin)
	INC+=-I ~/.brew/include -I/usr/local/include
	LINKER+=-framework OpenGL -L $(HOME)/.brew/lib/ -L./SOIL2-clone/ -lSOIL2 -framework CoreFoundation -L/usr/local/lib -lglfw3 -L$(LIBFT) -lft
else
	LINKER+=-L./SOIL2-clone -lglfw -lGL -lSOIL2 -L$(LIBFT) -lft
endif
NAME=scope
SRC=main.c events.c display.c parser.c fixcenter.c parser_count.c \
	parse_calc_size.c run.c maker.c parse_face.c attributes.c
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

fcleanall: fclean
	make -C $(DRAW) fclean
	make -C $(LIBFT) fclean

reall: fcleanall all

re: fclean all

$(LIBFT)/libft.a:
	make -j CC=clang OPENGL_ENABLED=yes BTREE= -C $(LIBFT)

$(DRAW)/libdraw.a: $(LIBFT)/libft.a
	make -j -C $(DRAW)

./SOIL2-clone/Makefile:
	git submodule init
	git submodule update

$(SOIL): ./SOIL2-clone/Makefile
	make -C ./SOIL2-clone/
