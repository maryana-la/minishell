all:
	gcc -g commands.c errors.c main.c preparser.c execve_com.c pipe.c libft/libft.a -lreadline -ominishell -L/Users/jjacquel/.brew/Cellar/readline/8.1/lib/ -I/Users/jjacquel/.brew/Cellar/readline/8.1/include

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all