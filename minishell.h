/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:20:33 by jjacquel          #+#    #+#             */
/*   Updated: 2021/07/07 23:39:40 by quadify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include "libft/libft.h"
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <signal.h>
# define GREEN "\001\033[32m\002"
# define RESET   "\001\033[0m\002"

int	g_status_exit_code;

typedef struct s_env
{
	char		*key;
	char		*value;
}				t_env;

typedef struct s_cmnd
{
	char	**args;
	int		fd_in;
	int		fd_out;
}				t_cmnd;

typedef struct s_all
{
	t_env	*env_vars;
	t_env	*env_sorted;
	t_cmnd	*cmnd;
	char	cwd[1000];
	char	*tmp_cwd;
	char	**envp;
	int		env_counter;
	int		num_of_pipes;
	int		pip_count;
	int		i;
	int		arg_pos;
	int		fd[2];
	int		fd_std[2];
	int		fd_tmp;
	int		ravno;
}				t_all;

/*
 * 	main
 */
int		take_input(t_all *all, char **str);
void	ft_free_commands(t_all *all);
void	init_all(t_all *all);

/*
 * 	preparser
 */
int		ft_preparser(char *str, t_all *all);
int		ft_preparser_1(char *str, t_all *all, int i);
int		preparser_error(char *token, int len);

/*
 * 	preparser_utils
 */
int		check_tokens(char *str, int *i, char token);
int		check_tokens_1(char *str, int *i, char token);
int		check_inside_d_quote(char *str, int *i);
int		check_inside_s_quote(char *str, int *i);

/*
 * 	env_init
 */
void	env_init(t_all *all, char **env);
void	add_variable(t_all *all, char *line, int i);
void	set_pwd(t_all *all);
void	set_oldpwd(t_all *all);

/*
 * 	env_shlvl
 */
void	increment_shlvl(char *line, int i, t_all *all, int *flag);
void	set_shlvl(t_all *all);

/*
 * 	signals
 */
void	sig_handler(int sig_id);
void	get_exit_status(int wstat);

/*
 * 	parser
 */
void	parse_and_exec(char *str, t_all *all);
void	ft_parse(t_all *all, char *str, int i);
void	get_all_args(t_all *all, char *str, int *i);
void	get_argument(const char *str, int *i, char *tmp, int *j);
void	ft_put_str_to_struct(char *arg, t_all *all);

/*
 * 	parser_utils
 */
int		get_arg_len(char *str, int i);
void	count_inside_quotes(const char *str, int *i, int *len, int type);
int		check_set(char c, const char *set);
void	skip_spaces(const char *str, int *i);
void	find_next_cmnd(const char *str, int *i);

/*
 * 	dollar
 */
char	*replace_env_with_value(char *str, int i, t_all *all);
char	*ft_dollar(char *str, int *i, t_all *all);
char	*get_value(t_all *all, char *var);
char	*place_value(char *str, int pos_of_dollar, int *i, char *value);
char	*return_str(int env, char *str);

/*
 * 	dollar_utils
 */
char	*if_question_mark(char *str, int pos_of_dollar, int *i, char *var);
char	*if_numeric(char *str, int pos_of_dollar, int *i, char *var);

/*
 * 	redirect
 */
int		ft_handle_redirect(char *str, int *i, t_all *all);
int		redirect_out_1(t_all *all, int *i, char *str);
int		redirect_out_2(t_all *all, int *i, char *str);
int		redirect_in_1(t_all *all, int *i, char *str);
int		redirect_in_2(t_all *all, int *i, char *str);

/*
 * 	redirect_utils
 */
int		check_open(t_all *all, char *heredoc_file);
void	read_stdin(t_all *all, char *stop);
int		heredoc_stdin_read(t_all *all, char *stop);

/*
 * 	redirect_utils2
 */
void	find_end_of_word(char *str, int *x);
char	*cut_name_from_str(char *str_new, int *i, int x, int len);
char	*get_file_name(char *str, int *i, int type, t_all *all);

/*
 * 	launch_commands
 */
void	launch_commands(t_all *all);
void	start_commands(t_all *all);
void	buitin_commands(t_all *all);
int		is_builtin(char *command);
void	start_commands_pipes(t_all *all);

/*
 * 	execution
 */
void	cmd_exec_child(t_all *all);
void	cmd_exec(t_all *all);
char	*sort_out_path(t_all *all, int i);
char	*get_data_path(t_all *all);
void	envs_list_to_array(t_all *all);

/*
 * 	exec_utils
 */
void	dup2_if_redirect(t_all *all);
void	replace_fds_child(t_all *all);
void	handle_fds_parent(t_all *all);
void	fork_error(t_all *all, pid_t *pid);
void	wait_children(t_all *all, pid_t *pid);

/*
 * 	cd_pwd_commands
 */
void	cd_command(t_all *all);
void	cd_command_1(t_all *all);
void	pwd_command(t_all *all);

/*
 * 	echo_command
 */
void	echo_command(t_all *all);
void	echo_command_1(t_all *all, int i);

/*
 * 	env_command
 */
void	print_env_list(t_env *for_print, int declare, int num_of_vars);
void	print_env_list_1(t_env *for_print, int i);

/*
 * 	exit_command
 */
void	exit_command(t_all *all);
void	exit_command_1(t_all *all);
void	exit_command_2(t_all *all);
void	print_and_exit(t_all *all, int err);

/*
 * 	export_command
 */
void	export_command(t_all *all);
void	sort_envs(t_all *all);
void	sort_envs_1(t_all *all, int i);

/*
 * 	export_command_utils
 */
void	add_new_variable(t_all *all);
int		add_new_variable_1(t_all *all);
int		add_new_variable_2(t_all *all, char *temp_key, char *temp_value);
void	add_new_variable_3(t_all *all, char *temp_key, char *temp_value);
int		add_new_variable_4(t_all *all, int *i);

/*
 * 	unset_command
 */
void	unset_command(t_all *all);
int		unset_command_1(t_all *all, int j);
void	unset_command_2(t_all *all, int i);

/*
 * 	errors
 */
void	error_handler(char *arg, int errorcode);
void	*ft_memdel(void *ptr);
void	ft_memdel_double(char **arr);
void	exec_error_print(char *cmnd, char *error);
void	ft_free_env(t_env *env);
void	rl_replace_line(const char *text, int clear_undo);

#endif //MINISHELL_MINISHELL_H
