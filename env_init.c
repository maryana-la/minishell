#include "minishell.h"

void	increment_shlvl(char *line, int i, t_all *all, int *flag)
{
	int		shlvl_tmp;
	char	*tmp_str;

	all->env_vars[i].key = ft_strdup("SHLVL");
	tmp_str = ft_substr(line, 6, (ft_strlen(line) - 6));
	shlvl_tmp = ft_atoi(tmp_str) + 1;
	if (shlvl_tmp < 0)
		shlvl_tmp = 0;
	else if (shlvl_tmp > 1000)
	{
		printf("minishell: warning: shell level (%d) too high, resetting to 1", \
			shlvl_tmp);
		shlvl_tmp = 1;
	}
	all->env_vars[i].value = ft_itoa(shlvl_tmp);
	ft_memdel(tmp_str);
	*flag = 1;
}

void	add_variable(t_all *all, char *line, int i)
{
	int	j;

	j = -1;
	while (line[++j] != '\0')
	{
		if (line[j] == '=')
			break ;
	}
	all->env_vars[i].key = ft_substr(line, 0, j);
	all->env_vars[i].value = ft_substr(line, j + 1, (ft_strlen(line) - j + 1));
}

void	set_oldpwd(t_all *all)
{
	int		i;
	t_env	*tmp;

	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "OLDPWD", 7) == 0)
		{
			ft_memdel(all->env_vars[i].value);
			all->env_vars[i].value = ft_strdup("nullvalue");
			return ;
		}
	}
	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup("OLDPWD");
	tmp[i].value = ft_strdup("nullvalue");
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
}

void	set_pwd(t_all *all)
{
	int		i;
	t_env	*tmp;

	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "PWD", 4) == 0)
		{
			ft_memdel(all->env_vars[i].value);
			all->env_vars[i].value = getcwd(NULL, 0);
			return ;
		}
	}
	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup("PWD");
	tmp[i].value = getcwd(NULL, 0);
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
	set_oldpwd(all);
}

void	set_shlvl(t_all *all)
{
	int		i;
	t_env	*tmp;

	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup("SHLVL");
	tmp[i].value = ft_strdup("1");
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
}

void	env_init(t_all *all, char **env)
{
	int	shlvl_flag;
	int	i;

	i = -1;
	shlvl_flag = 0;
	while (env[++i])
	{
	}
	all->env_vars = malloc(sizeof (t_env) * (i + 1));
	if (!all->env_vars)
		exit (errno);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			increment_shlvl(env[i], i, all, &shlvl_flag);
		else
			add_variable(all, env[i], i);
	}
	all->env_counter = i;
	all->env_vars[i].key = NULL;
	all->env_vars[i].value = NULL;
	set_pwd(all);
	if (shlvl_flag == 0)
		set_shlvl(all);
}
