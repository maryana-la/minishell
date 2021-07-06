all:
	gcc -g commands.c errors.c main.c preparser.c execve_com.c libft/libft.a -lreadline -ominishell

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all