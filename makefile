all:
	gcc -g commands.c errors.c main.c preparser.c execve_com.c pipe.c redirects.c libft/libft.a \
		-lreadline -ominishell -L ~/.brew/Cellar/readline/8.1/lib/ \
		-I ~/.brew/Cellar/readline/8.1/include

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all