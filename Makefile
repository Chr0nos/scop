# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#*   Updated: 2017/07/02 23:43:45 by snicolet         ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
DEBUG=0
CC=clang -O2 -march=native -mtune=native
FLAGS=-Werror -Wextra -Wall -Weverything -Wno-reserved-id-macro -Wno-documentation -Wno-documentation-unknown-command -Wno-padded
ifeq ($(DEBUG), 1)
	FLAGS += -g3
endif
DRAW=libdraw
LIBFT=libft
LINKER=-L$(DRAW) -lm -ldraw -Lglfw/src/ -lglfw3
INC=-I$(DRAW)/headers -I $(LIBFT)/ -Iheaders -Iglew/include -Ilibtga/includes -Iglfw/include
ifeq ($(OS),Darwin)
	INC+=-I ~/.brew/include -I/usr/local/include
	LINKER+=-framework OpenGL -L./SOIL2-clone/ -framework CoreFoundation -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -L$(LIBFT) -lft -L ~/.brew/lib/ -lGLEW
else
	LINKER+=-lGL -ldl -lpthread -L$(LIBFT) -lft -lX11 -lGLEW -lXrandr -lXinerama -lXcursor
endif
LINKER+=-Llibtga -ltga
NAME=scop
SRC=main.c events.c display.c parser.c parser_duplicate.c fixcenter.c \
	parser_count.c run.c maker.c parse_face.c attributes.c uniforms.c \
	mouse.c reset.c scroll.c light.c matrix.c parse_postprocess.c parse_real.c \
	configure.c resize.c
SRC_DIR=srcs
OBJ=$(SRC:%.c=$(BUILDDIR)/%.o)
BUILDDIR=build

all: $(NAME)

$(BUILDDIR):
	mkdir -p $@

$(NAME): $(DRAW)/libdraw.a ./glfw/src/libglfw3.a ./libtga/libtga.a $(BUILDDIR) $(OBJ)
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
	make -C libtga fclean
	make -C ./glfw/ clean

reall: fcleanall all

re: fclean all

$(LIBFT)/libft.a:
	make -j CC=clang OPENGL_ENABLED=yes BTREE= -C $(LIBFT)

$(DRAW)/libdraw.a: $(LIBFT)/libft.a
	make -j -C $(DRAW)

./libtga/libtga.a:
	make -C libtga

./glfw/src/libglfw3.a:
	cd glfw && cmake -D-DBUILD_SHARED_LIBS=OFF . && make
