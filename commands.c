#include "minishell.h"

void	buitin_commands(t_all *all)
{
	if (all->pip_count == 0)
		dup2_if_redirect(all);
	if (!ft_strncmp(all->cmnd[all->i].args[0], "pwd", 4))
		pwd_command(all);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "env", 4))
		print_env_list(all->env_vars, 0, all->env_counter);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "export", 7))
		export_command(all);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "unset", 6))
		unset_command(all);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "cd", 3))
		cd_command(all);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "echo", 5))
		echo_command(all);
	else if (!ft_strncmp(all->cmnd[all->i].args[0], "exit", 5))
		exit_command(all);
}

int	is_builtin(char *command)
{
	if (!ft_strncmp(command, "pwd", 4) || !ft_strncmp(command, "env", 4) || \
		!ft_strncmp(command, "export", 7) || !ft_strncmp(command, "unset", 6) \
		|| !ft_strncmp(command, "cd", 3) || !ft_strncmp(command, "echo", 5) \
		|| !ft_strncmp(command, "exit", 5))
		return (1);
	return (0);
}

void	start_commands(t_all *all)
{
	if (is_builtin(all->cmnd[all->i].args[0]))
		buitin_commands(all);
	else
	{
		if (all->pip_count == 0)
			cmd_exec(all);
		else
			cmd_exec_child(all);
	}

}

void	print_and_exit(t_all *all, int err)
{
	if (all->num_of_pipes == 0)
		ft_putendl_fd("exit", 2);
	exit(err);
}

void	exit_command_1(t_all *all)
{
	int	i;

	i = -1;
	while (all->cmnd[all->i].args[1][++i])
	{
		if (all->cmnd[all->i].args[1][i] == '-' && i == 0)
			continue ;
		if (!ft_isdigit(all->cmnd[all->i].args[1][i]))
		{
			ft_putendl_fd("exit", 2);
			exec_error_print(all->cmnd[all->i].args[1], \
				"numeric argument required");
			ft_free_env(all->env_vars);
			exit (255);
		}
	}
}

void	exit_command_2(t_all *all)
{
	int	error_code;

	error_code = ft_atoi(all->cmnd[all->i].args[1]);
	if (error_code < 0)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, 256 + error_code % 256);
	}
	else if (error_code < 256)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, error_code);
	}
	else
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, error_code % 256);
	}
}

void	exit_command(t_all *all)
{
	int	i;

	i = -1;
	while (all->cmnd[all->i].args[++i])
		;
	if (i == 1)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, 0);
	}
	if (i > 2)
	{
		exec_error_print("exit", "too many arguments");
		g_status_exit_code = 1;
		return ;
	}
	exit_command_1(all);
	exit_command_2(all);
}

void echo_command_1(t_all *all, int i)
{
	while (all->cmnd[all->i].args[++i])
	{
		if (all->cmnd[all->i].args[i + 1])
		{
			ft_putstr_fd(all->cmnd[all->i].args[i], 1);
			ft_putstr_fd(" ", 1);
		}
		else
			ft_putstr_fd(all->cmnd[all->i].args[i], 1);
	}
}

void echo_command(t_all *all)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	flag = 0;
	while (all->cmnd[all->i].args[++i])
	{
		if (!(ft_strncmp(all->cmnd[all->i].args[i], "-n", 2)))
		{
			j = 0;
			while (all->cmnd[all->i].args[i][++j] == 'n')
				;
			if (j == ft_strlen(all->cmnd[all->i].args[i]))
				flag = 1;
			else
				break ;
		}
		else
			break ;
	}
	i--;
	echo_command_1(all, i);
	if (!flag)
		ft_putstr_fd("\n", 1);
}

void cd_command_1(t_all *all)
{
	int		i;
	char	*tmp;

	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "OLDPWD"))
	{
	}
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(all->tmp_cwd);
		ft_memdel (tmp);
	}
	ft_memdel(all->tmp_cwd);
	getcwd(all->cwd, sizeof(all->cwd));
	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "PWD"))
		;
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(all->cwd);
		ft_memdel (tmp);
	}
}

void cd_command(t_all *all)
{
	int		i;

	i = -1;
	all->tmp_cwd = getcwd(NULL, 0);
	if (!all->cmnd[all->i].args[1])
	{
		while (++i < all->env_counter && \
			ft_strcmp(all->env_vars[i].key, "HOME"))
			;
		if (i != all->env_counter)
		{
			chdir(all->env_vars[i].value);
		}
	}
	else if (chdir(all->cmnd[all->i].args[1]) == -1)
	{
		printf("minishell: cd: %s: %s\n", \
			all->cmnd[all->i].args[1], strerror(errno));
		ft_memdel(all->tmp_cwd);
		g_status_exit_code = 1;
		return ;
	}
	cd_command_1(all);
}

void	pwd_command (t_all *all)
{
	getcwd(all->cwd, sizeof(all->cwd));
	write(1, all->cwd, ft_strlen(all->cwd));
	write(1, "\n", 1);
}

void export_command(t_all *all)
{
	all->arg_pos = 0;
	if (!all->cmnd[all->i].args[1])
	{
		sort_envs(all);
		print_env_list(all->env_sorted, 1, all->env_counter);
		ft_memdel(all->env_sorted);
		return ;
	}
	while (all->cmnd[all->i].args[++all->arg_pos])
	{
		add_new_variable(all);
	}
}

void add_new_variable_3(t_all *all, char *temp_key, char *temp_value)
{
	int		i;
	t_env	*tmp;

	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup(temp_key);
	if ((!temp_value) && !all->ravno)
		tmp[i].value = ft_strdup("nullvalue");
	else
		tmp[i].value = ft_strdup(temp_value);
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
	ft_memdel(temp_key);
	ft_memdel(temp_value);
	g_status_exit_code = 0;
}

int add_new_variable_2(t_all *all, char *temp_key, char *temp_value)
{
	char	*to_free;
	int		i;

	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, temp_key))
		;
	if (i != all->env_counter && (all->ravno == 0))
	{
		g_status_exit_code = 0;
		ft_memdel(temp_key);
		return (-1);
	}
	if (i != all->env_counter)
	{
		to_free = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(temp_value);
		ft_memdel(temp_key);
		ft_memdel(temp_value);
		ft_memdel(to_free);
		g_status_exit_code = 0;
		return (-1);
	}
	return (1);
}

int add_new_variable_1(t_all *all)
{
	if (!ft_isalpha(all->cmnd[all->i].args[all->arg_pos][0]) && \
	(all->cmnd[all->i].args[all->arg_pos][0] != '_'))
	{
		error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
		g_status_exit_code = 1;
		return (-1);
	}
	return (1);
}

int add_new_variable_4(t_all *all, int *i)
{
	*i = -1;
	while (all->cmnd[all->i].args[all->arg_pos][++(*i)])
	{
		if (all->cmnd[all->i].args[all->arg_pos][*i] == '=')
		{
			all->ravno = 1;
			break ;
		}
		else if (!ft_isalnum(all->cmnd[all->i].args[all->arg_pos][*i]) \
		&& (all->cmnd[all->i].args[all->arg_pos][0] != '_'))
		{
			error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
			g_status_exit_code = 1;
			return (-1);
		}
	}
	return (1);
}

void add_new_variable(t_all *all)
{
	int		i;
	char	*temp_key;
	char	*temp_value;

	if (add_new_variable_1(all) == -1)
		return ;
	all->ravno = 0;
	if (add_new_variable_4(all, &i) == -1)
		return ;
	temp_key = ft_substr(all->cmnd[all->i].args[all->arg_pos], 0, i);
	temp_value = NULL;
	if (all->ravno != 0)
		temp_value = ft_substr(all->cmnd[all->i].args[all->arg_pos], i + 1, \
		ft_strlen(all->cmnd[all->i].args[all->arg_pos]) - i + 1);
	if (add_new_variable_2(all, temp_key, temp_value) == -1)
		return ;
	add_new_variable_3(all, temp_key, temp_value);
}

void sort_envs_1(t_all *all, int i)
{
	t_env	tmp;

	tmp = all->env_sorted[i];
	all->env_sorted[i] = all->env_sorted[i + 1];
	all->env_sorted[i + 1] = tmp;
}

void sort_envs(t_all *all)
{
	int		i;
	int		z;
	int		j;

	i = -1;
	z = -1;
	all->env_sorted = malloc(sizeof (t_env) * (all->env_counter + 1));
	while (all->env_vars[++i].key)
		all->env_sorted[i] = all->env_vars[i];
	while (++z < all->env_counter - 2)
	{
		i = all->env_counter - 1;
		while (--i > z - 1)
		{
			j = 0;
			while (all->env_sorted[i].key[j] == all->env_sorted[i + 1].key[j])
				j++;
			if (all->env_sorted[i].key[j] > all->env_sorted[i + 1].key[j])
				sort_envs_1(all, i);
		}
	}
}

void print_env_list_1(t_env *for_print, int i)
{
	write (1, "declare -x ", 11);
	write(1, for_print[i].key, ft_strlen(for_print[i].key));
	if (ft_strcmp(for_print[i].value, "nullvalue"))
	{
		write(1, "=", 1);
		write(1, "\"", 1);
		write(1, for_print[i].value, ft_strlen(for_print[i].value));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void print_env_list(t_env *for_print, int declare, int num_of_vars)
{
	int	i;

	i = 0;
	while (i < num_of_vars && for_print[i].key && for_print[i].key[0] != '\0')
	{
		if (declare)
			print_env_list_1(for_print, i);
		else
		{
			if (ft_strcmp(for_print[i].value, "nullvalue"))
			{
				write(1, for_print[i].key, ft_strlen(for_print[i].key));
				write(1, "=", 1);
				write(1, for_print[i].value, ft_strlen(for_print[i].value));
				write(1, "\n", 1);
			}
		}
		i++;
	}
}

int unset_command_1(t_all *all, int j)
{
	if (!ft_isalpha(all->cmnd[all->i].args[j][0]) && \
	(all->cmnd[all->i].args[j][0] != '_'))
	{
		error_handler(all->cmnd[all->i].args[j], 2);
		all->arg_pos = j;
		g_status_exit_code = 1;
		return (-1);
	}
	return (1);
}

void unset_command_2(t_all *all, int i)
{
	ft_memdel(all->env_vars[i].key);
	ft_memdel(all->env_vars[i].value);
	while (i < all->env_counter)
	{
		all->env_vars[i] = all->env_vars[i + 1];
		i++;
	}
}

void unset_command(t_all *all)
{
	int	i;
	int	j;

	j = 0;
	while (all->cmnd[all->i].args[++j])
	{
		i = -1;
		if (unset_command_1(all, j) == -1)
			return ;
		while (++i < all->env_counter && \
			ft_strcmp(all->env_vars[i].key, all->cmnd[all->i].args[j]))
			;
		if (i != all->env_counter)
		{
			unset_command_2(all, i);
			--all->env_counter;
			continue ;
		}
		else
			continue ;
	}
}

void sig_handler(int sig_id)
{
	int		code;

	wait(&code);
	if (code == sig_id)
	{
		if (sig_id == SIGINT)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else if (sig_id == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", 2);
		}
		g_status_exit_code = 128 + sig_id;
	}
	else if (sig_id == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_status_exit_code = 1;
	}
}
