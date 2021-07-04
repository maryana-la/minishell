all:
	gcc -g commands.c main.c preparser.c execve_com.c libft/libft.a -lreadline -ominishell

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all