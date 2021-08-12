NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRC :=	commands.c errors.c main.c preparser.c execve_com.c pipe.c redirects.c env_init.c ft_dollar.c

OBJ := ${SRC:.c=.o}

HEADERS = libft/libft.h minishell.h

LIBS = libft/libft.a -lreadline -L ~/.brew/Cellar/readline/8.1/lib/ -I ~/.brew/Cellar/readline/8.1/include

LIBFT = libft/libft.a

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): LIBFT $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

bonus: all

clean:
	rm -fr $(OBJ)

fclean: clean
	make clean -C libft/
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
