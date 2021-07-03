#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"
#include <string.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <editline/readline.h>


typedef struct s_env
{
	char		*key;
	char		*value;
	size_t 		key_len;
	size_t		value_len;
}				t_env;

typedef struct s_all
{
	t_env	*env_vars;
	t_env	*env_sorted;
	char	cwd[1000];
	char	**envp;
	char	**args;
	int		arg_len;
	int		env_counter;
}				t_all;


//Maryana`s func start
int ft_preparcer(char *str);
int check_inside_s_quote(char *str, int *i);
int check_inside_d_quote(char *str, int *i);
int check_tokens(char *str, int *i, char token);

/*
** main.c
*/

char *ft_slash(char *str, int *i);
int check_set(char c, char *set);
void skip_spaces(char *str, int *i);
char *ft_dollar(char *str, int *i, t_all *all);
// char *ft_s_quote(char *str, int *i, int *tmp);
void ft_s_quote(char *str, char **arg, int *i, int *j_tmp);
char *ft_double_quote(char *str, int *i, int *tmp);
int	find_enf_of_arg(char *str, int i, t_all *all);
char	*replace_env_with_value(char *str, t_all *all);
void ft_parcer(char *str, t_all *all);
int env_init(t_all *all, char **env);
void init_all(t_all *all);
void start_commands(t_all *all);

//Maryana`s func end

void	pwd_command (t_all *all);
void 	export_command(t_all *all);
//void 	env_init(t_all *all, char **env);
void print_env_list(t_env *for_print, int declare, int num_of_vars);
void	add_new_variable(t_all *all);
void	sort_envs(t_all *all);
void unset_command(t_all *all);
void cd_command(t_all *all);
void rl_replace_line();

#endif //MINISHELL_MINISHELL_H
