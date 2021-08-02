#include "minishell.h"

void start_commands(t_all *all)
{
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
	else
		cmd_exec(all);
}

void print_and_exit (t_all *all, int err)
{
	if (!all->num_of_pipes)
		printf("exit\n");
	exit(err);
}

void exit_command(t_all *all)
{
	int i;
	int error_code;

	i = -1;
	while(all->cmnd[all->i].args[++i]);
	if (i == 1)
		print_and_exit(all, 0);
	if (i > 2)
	{
		printf("minishell: exit: too many arguments\n");
		all->last_exit = 1;
		return ;
	}

	i = -1;
	while (all->cmnd[all->i].args[1][++i]) //check if only numbers
	{
		if (!ft_isdigit(all->cmnd[all->i].args[1][i]) || (all->cmnd[all->i].args[1][i] == '-' && i != 0))
		{
			printf("minishell: exit: %s: numeric argument required\n", all->cmnd[all->i].args[1]);
			exit (255);
		}
	}
	error_code = ft_atoi(all->cmnd[all->i].args[1]);
	if (error_code < 0)
		print_and_exit(all,256 + error_code % 256);
	else if (error_code < 256)
		print_and_exit(all, error_code);
	else
		print_and_exit(all,error_code % 256);
}

void echo_command(t_all *all)
{
	int i;
	int j;
	int flag;

	i = 0;
	flag = 0;

	while (all->cmnd[all->i].args[++i])
	{
		if (!(ft_strncmp(all->cmnd[all->i].args[i], "-n", 2)))
		{
			j = 0;
			while (all->cmnd[all->i].args[i][++j] == 'n');
			if (j == ft_strlen(all->cmnd[all->i].args[i]))
				flag = 1;
			else
				break;
		}
		else
			break ;
	}

	i--;
	while (all->cmnd[all->i].args[++i])
	{
		if (all->cmnd[all->i].args[i+1])
			printf("%s ", all->cmnd[all->i].args[i]);
		else
			printf("%s", all->cmnd[all->i].args[i]);
	}
	if (!flag)
		printf("\n");
}

void cd_command(t_all *all)
{
	int i;
	char *tmp;

	i = -1;
	all->tmp_cwd = getcwd(NULL, 0);
	if (chdir(all->cmnd[all->i].args[1]) == -1) //check if no error with folder
	{
		printf("minishell: cd: %s: %s\n", all->cmnd[all->i].args[1], strerror(errno));
		all->last_exit = 1;
		return ;
	}

//if PWD or OLDPWD unset - they don't appear
	while(++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "OLDPWD"));
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value; //to free malloc
		all->env_vars[i].value = ft_strdup(all->tmp_cwd);
		free (tmp);
	}

	getcwd(all->cwd, sizeof(all->cwd));
	i = -1;
	while(++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "PWD"));
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value; //to free malloc
		all->env_vars[i].value = ft_strdup(all->cwd);
		free (tmp);
	}
}

void pwd_command (t_all *all)
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
		return ;
	}

	while (all->cmnd[all->i].args[++all->arg_pos])
	{
		add_new_variable(all);
	}
}

void add_new_variable(t_all *all)
{
	int i;
	int j = -1;
	t_env *tmp;
	char *temp_key;
	char *temp_value;
	int ravno = 0;


	if (!ft_isalpha(all->cmnd[all->i].args[all->arg_pos][0]) && (all->cmnd[all->i].args[all->arg_pos][0] != '_'))
	{
		error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
		all->last_exit = 1;
		return ;
	}

	while (all->cmnd[all->i].args[all->arg_pos][++j])
	{
		if (all->cmnd[all->i].args[all->arg_pos][j] == '=')
		{
			ravno = 1;
			break ;
		}
		else if (!ft_isalnum(all->cmnd[all->i].args[all->arg_pos][j]) &&(all->cmnd[all->i].args[all->arg_pos][0] != '_'))
		{
			error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
			all->last_exit = 1;
			return ;
		}
	}
	temp_key = ft_substr(all->cmnd[all->i].args[all->arg_pos], 0, j);
	temp_value = ft_substr(all->cmnd[all->i].args[all->arg_pos], j + 1, ft_strlen(all->cmnd[all->i].args[1])-j+1);

	i=-1;
	while(++i < all->env_counter && ft_strcmp(all->env_vars[i].key, temp_key));

	if (i != all->env_counter && temp_value[0] == '\0' && !ravno)
	{
		all->last_exit = 0;
		return ;
	}

	if (i != all->env_counter)
	{
		all->env_vars[i].value = temp_value;
		all->last_exit = 0;
		return ;
	}

	i=-1;
	j=-1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	while (all->cmnd[all->i].args[all->arg_pos][++j])
	{
		if (all->cmnd[all->i].args[all->arg_pos][j] == '=')
			break;
	}
	tmp[i].key = ft_substr(all->cmnd[all->i].args[all->arg_pos], 0, j);
	tmp[i].value = ft_substr(all->cmnd[all->i].args[all->arg_pos], j + 1, ft_strlen(all->cmnd[all->i].args[1])-j+1);
	if ((!tmp[i].value) || (tmp[i].value[0] == '\0' && !ravno))
		tmp[i].value = ft_strdup("nullvalue");
	tmp[i].key_len = ft_strlen(tmp[i].key);
	tmp[i].value_len = ft_strlen(tmp[i].value);
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	free(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
	all->last_exit = 0;
}

void sort_envs(t_all *all)
{
	int i = -1;
	int z = -1;
	int j;

	all->env_sorted = malloc(sizeof (t_env) * (all->env_counter + 1));
	while (all->env_vars[++i].key)
		all->env_sorted[i] = all->env_vars[i];

	t_env tmp;
	while (++z < all->env_counter - 2)
	{
		i = all->env_counter - 1;
		while (--i > z - 1)
		{
			j = 0;
			while (all->env_sorted[i].key[j] == all->env_sorted[i+1].key[j])
				j++;
			if (all->env_sorted[i].key[j] > all->env_sorted[i+1].key[j])
			{
				tmp = all->env_sorted[i];
				all->env_sorted[i] = all->env_sorted[i + 1];
				all->env_sorted[i + 1] = tmp;
			}
		}
	}
}

void print_env_list(t_env *for_print, int declare, int num_of_vars)
{
	int i = 0;

	while (i < num_of_vars && for_print[i].key && for_print[i].key[0] != '\0')
	{
		if (declare)
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
			write(1, "\n",1);
		}
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

void unset_command(t_all *all)
{
	int i;
	int j = 0;

	while (all->cmnd[all->i].args[++j])
	{
		i = -1;

		if (!ft_isalpha(all->cmnd[all->i].args[j][0]) && (all->cmnd[all->i].args[j][0] != '_'))
		{
			error_handler(all->cmnd[all->i].args[j], 2);
			all->arg_pos = j;
			all->last_exit = 1;
			return ;
		}

		while(++i < all->env_counter && ft_strcmp(all->env_vars[i].key, all->cmnd[all->i].args[j]));
		if (i != all->env_counter)
		{
			while (i < all->env_counter)
			{
				all->env_vars[i] = all->env_vars[i+1];
				i++;
			}
			--all->env_counter;
			continue;
		}
		else
			continue;
		//todo зафришить удаленную переменную
	}
}

void sig_handler(int sig_id)
{
	if (sig_id == SIGINT)
	{
		printf("\n"); // Move to a new line
		rl_on_new_line(); // Regenerate the prompt on a newline
		rl_replace_line("", 0); // Clear the previous text
		rl_redisplay();
	}
	if (sig_id == SIGQUIT)
	{}
}
