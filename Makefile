# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/24 07:18:03 by snicolet          #+#    #+#              #
#    Updated: 2016/07/24 11:58:14 by snicolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=clang
FLAGS=-Werror -Wextra -Wall -Weverything
LINKER=-L../rt/libs/libdraw -lm -ldraw -lglut -lGL
INC=-I../rt/libs/libdraw/headers
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
