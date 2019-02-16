# q
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/29 15:05:00 by ehugh-be          #+#    #+#              #
#    Updated: 2018/10/29 17:52:30 by ehugh-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol
SRC = $(wildcard *.c) 
OBJ = $(SRC:.c=.o)
HDR = fractol.h
LIB = libft/libft.a
LIBDIR = libft/
FLAGS = -Wall -Wextra -Werror
MFL = -lmlx -framework OpenGL -framework AppKit -framework OpenCL
MLX = /usr/local/lib/
MLXH = /usr/local/include

all: lib $(NAME)

lib:
	make -C $(LIBDIR)

$(NAME): $(OBJ)
	gcc $(LIB) -I $(MLXH) -L $(MLX) $(MFL) $(OBJ) -o $(NAME)

%.o: %.c $(HDR) $(LIB)
	gcc $(FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) 

re: fclean all
