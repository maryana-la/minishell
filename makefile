all:
	gcc -g ../libft/libft.a minishell.c -ominishell

clean:
	rm -rf minishell.o

fclean: clean
	rm -rf minishell

re: fclean all