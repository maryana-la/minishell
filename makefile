all:
#	make -C libft/
#	rm -rf libft/*.o

#	gcc libft/libft.a -g commands.c errors.c redirects.c main.c preparser.c execve_com.c pipe.c  -lreadline -ominishell -L ~/.brew/Cellar/readline/8.1/lib/ -I ~/.brew/Cellar/readline/8.1/include -L /usr/local/Cellar/readline/8.0.4/lib/ -I /usr/local/Cellar/readline/8.0.4/include
	gcc -Wall -Wextra -Werror -g commands.c errors.c main.c preparser.c execve_com.c pipe.c redirects.c env_init.c ft_dollar.c libft/libft.a \
		-lreadline -ominishell -L ~/.brew/Cellar/readline/8.1/lib/ \
		-I ~/.brew/Cellar/readline/8.1/include
clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all
