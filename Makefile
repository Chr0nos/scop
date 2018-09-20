# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#*   Updated: 2018/03/09 18:03:51 by snicolet         ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

OS=$(shell uname -s)
DEBUG=0
CC=clang -O2 -march=native -mtune=native
FLAGS=-Werror -Wextra -Wall -Weverything -Wno-reserved-id-macro -Wno-documentation -Wno-documentation-unknown-command -Wno-padded
ifeq ($(DEBUG), 1)
	FLAGS += -g3
endif

DRAW=./libs/libdraw
LIBFT=./libs/libft
LIBTGA=./libs/libtga
GLFW=./libs/glfw
SOIL2=./libs/SOIL2-clone

LINKER=-L$(DRAW) -lm -ldraw -L$(GLFW)/src/ -lglfw3
INC=-I$(DRAW)/headers -I $(LIBFT)/include -I$(SOIL2)/incs -Iheaders -Iglew/include -I$(LIBTGA)/includes -I$(GLFW)/include
SOIL=$(SOIL2)/libSOIL2.a
ifeq ($(OS),Darwin)
	INC+=-I ~/.brew/include -I/usr/local/include
	LINKER+=-framework OpenGL -L$(SOIL2)/ -lSOIL2 -framework CoreFoundation -framework Cocoa -framework IOKit -framework CoreVideo -L/usr/local/lib -L$(LIBFT) -lft -L ~/.brew/lib/ -lGLEW
else
	LINKER+=-L$(SOIL2) -lGL -ldl -lpthread -lSOIL2 -L$(LIBFT) -lft -lX11 -lGLEW -lXrandr -lXinerama -lXcursor
endif
LINKER+=-L$(LIBTGA) -ltga
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

$(NAME): $(SOIL) $(DRAW)/libdraw.a $(GLFW)/src/libglfw3.a $(LIBTGA)/libtga.a $(BUILDDIR) $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LINKER)

$(BUILDDIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(INC) $(FLAGS) $< -o $@

$(LIBFT):
	git clone git@github.com:/Chr0nos/libft.git

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

$(LIBFT)/libft.so: ./libs/libft
	make -j CC=clang OPENGL_ENABLED=yes BTREE= -C $(LIBFT) so

$(LIBFT)/libft.a: ./libs/libft
	make -j CC=clang OPENGL_ENABLED=yes BTREE= -C $(LIBFT)

$(DRAW)/libdraw.a: $(LIBFT)/libft.a
	make -j -C $(DRAW)

$(LIBTGA)/libtga.a:
	make -C $(LIBTGA)

$(GLFW)/src/libglfw3.a:
	cd $(GLFW) && cmake -D-DBUILD_SHARED_LIBS=OFF . && make

$(SOIL): $(SOIL2)/Makefile
	make -C ./libs/SOIL2-clone/

$(SOIL2)/Makefile:
	git submodule init
	git submodule update
