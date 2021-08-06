#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include "libft/libft.h"
# include <string.h>
# include <fcntl.h>
//# include "/Users/jjacquel/.brew/Cellar/readline/8.1/include/readline/readline.h"
# include <readline/readline.h>
# include <readline/history.h>
//# include <editline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <signal.h>
//pid_t global_pid;
typedef struct s_env
{
	char		*key;
	char		*value;
	size_t 		key_len;
	size_t		value_len;
}				t_env;

typedef struct s_cmnd
{
	char	**args;
	int 	fd_in;
	int 	fd_out;
}				t_cmnd;

typedef struct s_all
{
	t_env	*env_vars;
	t_env	*env_sorted;
	t_cmnd 	*cmnd;
	char	cwd[1000];
	char 	*tmp_cwd;
	char	**envp;
	int		arg_len;
	int		env_counter;
	int 	num_of_pipes;
	int		pip_count;
	int		i;
	int 	arg_pos;
	int 	fd[2];
	int 	fd_std[2];
	int 	fd_tmp;
	int 	last_exit;
}				t_all;


//Maryana`s func start
int ft_preparser(char *str, t_all *all);
int check_inside_s_quote(char *str, int *i);
int check_inside_d_quote(char *str, int *i);
int check_tokens(char *str, int *i, char token);

/*
** main.c
*/

int takeInput(t_all *all, char** str);
int check_set(char c, char *set);
void skip_spaces(char *str, int *i);
char *ft_dollar(char *str, int *i, t_all *all);
char	*replace_env_with_value(char *str, t_all *all);
void	ft_parser(char *str, t_all *all);
void	env_init(t_all *all, char **env);
void	init_all(t_all *all);
void	start_commands(t_all *all);
int	get_arg_len(char *str, int i);

/*
 *  execve_com.c
 */

void	cmd_exec(t_all *all);
char	*get_data_path(t_all *all);
void	envs_list_to_array(t_all *all);
void 	cmd_exec1(t_all *all);

/*
 * 	pipe.c
 */

void 	launch_commands(t_all *all);

/*
 *  redirects
 */

char	*get_file_name(char *str, int *i, t_all *all);
void	heredoc_stdin_read(t_all *all, char *stop);
void	ft_handle_redirect(char *str, int *i, t_all *all);
void	*ft_memdel(void *ptr);

//Maryana`s func end

void	pwd_command (t_all *all);
void 	export_command(t_all *all);
void	print_env_list(t_env *for_print, int declare, int num_of_vars);
void	add_new_variable(t_all *all);
void	sort_envs(t_all *all);
void unset_command(t_all *all);
void cd_command(t_all *all);
void rl_replace_line();
void echo_command(t_all *all);
void exit_command(t_all *all);
void error_handler(char *arg, int errorcode);
void sig_handler(int sig_id);
void print_and_exit (t_all *all, int err);

#endif //MINISHELL_MINISHELL_H
