NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

RM		= rm -f

SRC := builtins/cd_pwd_commands.c builtins/env_command.c builtins/export_command.c builtins/unset_command.c \
		builtins/echo_command.c builtins/exit_command.c builtins/export_command_utils.c \
		exec/errors.c exec/exec_utils.c exec/execution.c exec/launch_commands.c exec/signals.c \
		parser/dollar.c parser/env_init.c parser/parser.c parser/preparser.c parser/redirect.c parser/redirect_utils2.c \
		parser/dollar_utils.c parser/env_shlvl.c parser/parser_utils.c parser/preparser_utils.c  parser/redirect_utils.c \

OBJ := $(SRC:.c=.o)

HEADERS = libft/libft.h minishell.h

LIBS = libft/libft.a -lreadline -L /opt/homebrew/Cellar/readline/8.2.1/lib/ -I /opt/homebrew/Cellar/readline/8.2.1/include
LIBFT = libft/libft.a

%.o : %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) main.c
	@$(CC) $(CFLAGS) $(LIBS) $(OBJ) main.c -o $(NAME)
	@echo Completed

$(LIBFT):
	@$(MAKE) -C libft

bonus: all

clean:
	@$(RM) $(OBJ)
	@make clean -C libft/

fclean: clean
	@make fclean -C libft/
	@$(RM) $(NAME)
	@echo Files removed

re: fclean all

.PHONY:	all clean fclean re bonus
