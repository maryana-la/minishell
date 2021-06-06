#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"
#include "minishell.h"



int main(int argc, char **argv, char **envp)
{
	t_all all;
	env_init(&all, envp);


	all.args[0] = "export";
//	all.args[1] = "temp=567";
//	all.args[1] = NULL;

	if (!ft_strncmp(all.args[0], "pwd", ft_strlen(all.args[0])))
		pwd_command(&all);
	else if (!ft_strncmp(all.args[0], "env", ft_strlen(all.args[0])))
		print_env_list(&all);
	else if (!ft_strncmp(all.args[0], "export", ft_strlen(all.args[0])))
		export_command(&all);

}

void env_init(t_all *all, char **env)
{
	t_env	*new;
	t_env	*allenv_vars;
	int i = -1;
	int j;

	while (env[++i]);

	all->env_vars = malloc(sizeof(t_env) * (i + 1));
	if (!all->env_vars)
		return;
	i = -1;
	while(env[++i])
	{
		j = -1;
		while(env[i][++j] != '\0')
			if (env[i][j] == '=')
				break;
		all->env_vars[i].key = ft_substr(env[i], 0, j);
		all->env_vars[i].value = ft_substr(env[i], j + 1, (ft_strlen(env[i]) - j + 1));
	}
	all->env_counter = i;
}

void print_env_list(t_all *all)
{
	int i = -1;

	while (all->env_vars[++i].key && ft_strncmp((all->env_vars[i].key), "\0", ft_strlen(all->env_vars[i].key)))
	{
		write(1, all->env_vars[i].key, ft_strlen(all->env_vars[i].key));
		write(1, "=", 1);
		write(1, all->env_vars[i].value, ft_strlen(all->env_vars[i].value));
		write(1, "\n",1);
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
		while (all->env_vars[++i].key)
		{
			write(1, "declare -x ", 11);
			write(1, all->env_vars[i].key, ft_strlen(all->env_vars[i].key));
			write(1, "=", 1);
			write(1, all->env_vars[i].value, ft_strlen(all->env_vars[i].value));
			write(1, "\n", 1);
		}
		exit(0);
	}
	else
	{
		add_new_variable(all);
		print_env_list(all);
	}
}

void	add_new_variable(t_all *all)
{
	int i = -1;
	int j = -1;
	t_env *tmp;

	tmp = malloc(sizeof (t_env) * (all->env_counter + 1));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	while (all->args[1][++j])
	{
		if (all->args[1][j] == '=')
			break;
	}
	tmp[i].key = ft_substr(all->args[1], 0, j);
	tmp[i].value = ft_substr(all->args[1], j + 1, ft_strlen(all->args[1])-j+1);
	free(all->env_vars);
	all->env_vars = tmp;
}