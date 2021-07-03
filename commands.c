#include "minishell.h"

void print_echo(t_all *all)
{
	int i;

	i = 0;

	while (all->args[++i])
	{

		if (all->args[i+1])
			printf("%s ", all->args[i]);
		else
			printf("%s", all->args[i]);
	}


}

void start_commands(t_all *all)
{

	if (!ft_strncmp(all->args[0], "pwd", 4))
		pwd_command(all);
	else if (!ft_strncmp(all->args[0], "env", 4))
		print_env_list(all->env_vars, 0, all->env_counter);
	else if (!ft_strncmp(all->args[0], "export", 7))
		export_command(all);
	else if (!ft_strncmp(all->args[0], "unset", 6))
		unset_command(all);
	else if (!ft_strncmp(all->args[0], "cd", 3))
		cd_command(all);
	else if (!ft_strncmp(all->args[0], "echo", 5))
		print_echo(all);
}

void cd_command(t_all *all)
{
	chdir(all->args[1]);
}

//void env_init(t_all *all, char **env)
//{
//	int i = -1;
//	int j;
//
//	while (env[++i]);
//	all->env_vars = malloc(sizeof(t_env) * (i + 1));
//	if (!all->env_vars)
//		return;
//	i = -1;
//	while(env[++i])
//	{
//		j = -1;
//		while(env[i][++j] != '\0')
//			if (env[i][j] == '=')
//				break;
//		all->env_vars[i].key = ft_substr(env[i], 0, j);
//		all->env_vars[i].key_len = ft_strlen(all->env_vars[i].key);
//		all->env_vars[i].value = ft_substr(env[i], j + 1, (ft_strlen(env[i]) - j + 1));
//		all->env_vars[i].value_len = ft_strlen(all->env_vars[i].value);
//	}
//	all->env_counter = i;
//	all->env_vars[i].key = "\0";
//	all->env_vars[i].value = "\0";
//}

void print_env_list(t_env *for_print, int declare, int num_of_vars)
{
	int i = 0;

	while (i < num_of_vars && for_print[i].key && for_print[i].key[0] != '\0')
	{
		if (declare)
			write (1, "declare -x ", 11);
		write(1, for_print[i].key, ft_strlen(for_print[i].key));
		write(1, "=", 1);
		write(1, for_print[i].value, ft_strlen(for_print[i].value));
		write(1, "\n",1);
		i++;
	}
}



void pwd_command (t_all *all)
	{
		getcwd(all->cwd, sizeof(all->cwd));
		write(1, all->cwd, ft_strlen(all->cwd));
	}

void export_command(t_all *all)
{
	int i = -1;

	if (!all->args[1])
	{
		sort_envs(all);
		print_env_list(all->env_sorted, 1, all->env_counter);
	}
	else
	{
		add_new_variable(all);
//		sort_envs(all);
//		print_env_list(all, 1);
	}
}

void	add_new_variable(t_all *all)
{
	int i = -1;
	int j = -1;
	t_env *tmp;

	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	while (all->args[1][++j])
	{
		if (all->args[1][j] == '=')
			break;
	}
	tmp[i].key = ft_substr(all->args[1], 0, j);
	tmp[i].value = ft_substr(all->args[1], j + 1, ft_strlen(all->args[1])-j+1);
	tmp[i].key_len = ft_strlen(tmp[i].key);
	tmp[i].value_len = ft_strlen(tmp[i].value);
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	free(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
}

void unset_command(t_all *all)
{
	int i;
	int j = 0;

	while (all->args[++j])
	{
		i = -1;

		while (!ft_strncmp(all->env_vars[++i].key, all->args[j], ft_strlen(all->args[j]) + 1));
		while (i < all->env_counter)
		{
			all->env_vars[i] = all->env_vars[i+1];
			i++;
		}
		if (i != all->env_counter - 1)
			--all->env_counter;
	//todo зафришить удаленную переменную
	}

}



void sort_envs(t_all *all)
{
	int i = -1;
	int z = -1;
	int j = -1;

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
//				tmp.key = all->env_sorted[i].key;
//				tmp.value = all->env_sorted[i].value;
//				all->env_sorted[i].key = all->env_sorted[i + 1].key;
//				all->env_sorted[i].value = all->env_sorted[i + 1].value;
//				all->env_sorted[i + 1].key = tmp.key;
//				all->env_sorted[i + 1].value = tmp.value;
			}
		}
	}
}