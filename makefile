all:
	gcc -g ../libft/libft.a commands.c main.c preparser.c  -ominishell -ledit

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all