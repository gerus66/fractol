OS = $(shell uname)

NAME = fractol

SRC = $(wildcard ./srcs/*.c) 
OBJ = $(SRC:.c=.o)
HDR = ./srcs/fractol.h

LIB = ./lib
INC = ./includes

LIBFT = $(LIB)/libft.a

ifeq ($(OS), Darwin)
	LIBMLX = $(LIB)/mlx_macos.a
	W_FLAGS = -Wall -Wextra -Werror
	FLAGS = -framework OpenGL -framework AppKit
	FLAGS += -framework OpenCL
	CFLAGS = -DMAC
else
ifeq ($(OS), Linux)
# TODO
#	LIBMLX = $(LIB)/mlx_linux.a
#	FLAGS = -lmlx -lXext -lX11
#	FLAGS += -lOpenCL
else
	@echo "Cant make it on your system, sorry.."
endif
endif

all: $(NAME)

$(NAME): $(OBJ)
	gcc -I $(INC) $(LIBFT) $(LIBMLX) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HDR)
	gcc -I $(INC) $(CFLAGS) $(W_FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) 

re: fclean all
