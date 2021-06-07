#ifndef MINISHELL_MINISHELL_H
#define MINISHELL_MINISHELL_H

typedef struct s_env
{
	char		*key;
	char		*value;
	int			key_len;
	int			value_len;
}				t_env;

typedef struct s_all
{
	t_env	*env_vars;
	t_env	*env_sorted;
	char	cwd[1000];
	char	**envp;
	char	*args[1000];
	int		arg_len;
	int		env_counter;
}				t_all;


void	pwd_command (t_all *all);
void 	export_command(t_all *all);
void 	env_init(t_all *all, char **env);
void print_env_list(t_all *all, int declare);
void	add_new_variable(t_all *all);
void	sort_envs(t_all *all);

#endif //MINISHELL_MINISHELL_H
