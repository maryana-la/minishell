#include "minishell.h"

void env_init(t_all *all, char **env) // env init with lists/ leaks done
{
	int shlvl_tmp;
	int pwd_flag = 0;
	int oldpwd_flag = 0;
	int shlvl_flag = 0;

	char *tmp_str;

	int i = -1;
	int j;

	while (env[++i]);
	all->env_vars = malloc(sizeof(t_env) * (i + 1));
	if (!all->env_vars)
		exit (-2);
	i = -1;
	while(env[++i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
		{
			all->env_vars[i].key = ft_strdup("PWD");
			all->env_vars[i].key_len = ft_strlen(all->env_vars[i].key);
			all->env_vars[i].value = getcwd(NULL, 0);
			all->env_vars[i].value_len = ft_strlen(all->env_vars[i].value);
			pwd_flag = 1;
		}
		else if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			all->env_vars[i].key = ft_strdup("OLDPWD");
			all->env_vars[i].key_len = ft_strlen(all->env_vars[i].key);
			all->env_vars[i].value = ft_strdup("nullvalue");
			all->env_vars[i].value_len = 0;
			oldpwd_flag = 1;
		}
		else if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			all->env_vars[i].key = ft_strdup("SHLVL");
			all->env_vars[i].key_len = ft_strlen(all->env_vars[i].key);
			tmp_str = ft_substr(env[i], 6, (ft_strlen(env[i]) - 6));
			shlvl_tmp = ft_atoi(tmp_str) + 1;
			if (shlvl_tmp < 0)
				shlvl_tmp = 0;
			if (shlvl_tmp > 1000)
			{
				printf("minishell: warning: shell level (%d) too high, resetting to 1", shlvl_tmp);
				shlvl_tmp = 1;
			}
			all->env_vars[i].value = ft_itoa(shlvl_tmp);
			ft_memdel(tmp_str);
			all->env_vars[i].value_len = ft_strlen(all->env_vars[i].value);
			shlvl_flag = 1;
		}
		else
		{
			j = -1;
			while (env[i][++j] != '\0')
				if (env[i][j] == '=')
					break;
				all->env_vars[i].key = ft_substr(env[i], 0, j);
				all->env_vars[i].key_len = ft_strlen(all->env_vars[i].key);
				all->env_vars[i].value = ft_substr(env[i], j + 1, (ft_strlen(env[i]) - j + 1));
				all->env_vars[i].value_len = ft_strlen(all->env_vars[i].value);
		}
	}
	all->env_counter = i;
	all->env_vars[i].key = NULL;
	all->env_vars[i].value = NULL;

	// if no SHVLV PWD OLDPWD- set it to 1
	if(shlvl_flag == 0)
	{
		i = -1;
		t_env *tmp;

		tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
		while (all->env_vars[++i].key)
			tmp[i] = all->env_vars[i];
		tmp[i].key = ft_strdup("SHLVL");
		tmp[i].value = ft_strdup("1");
		tmp[i].key_len = ft_strlen(tmp[i].key);
		tmp[i].value_len = ft_strlen(tmp[i].value);
		tmp[i + 1].key = NULL;
		tmp[i + 1].value = NULL;
		ft_memdel(all->env_vars);
		all->env_vars = tmp;
		all->env_counter++;
	}
	if(pwd_flag == 0)
	{
		i = -1;
		t_env *tmp;

		tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
		while (all->env_vars[++i].key)
			tmp[i] = all->env_vars[i];
		tmp[i].key = ft_strdup("PWD");
		tmp[i].key_len = ft_strlen(tmp[i].key);
		tmp[i].value = getcwd(NULL, 0);
		tmp[i].value_len = ft_strlen(tmp[i].value);
		tmp[i + 1].key = NULL;
		tmp[i + 1].value = NULL;
		ft_memdel(all->env_vars);
		all->env_vars = tmp;
		all->env_counter++;
	}
	if(oldpwd_flag == 0)
	{
		i = -1;
		t_env *tmp;

		tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
		while (all->env_vars[++i].key)
			tmp[i] = all->env_vars[i];
		tmp[i].key = ft_strdup("OLDPWD");
		tmp[i].key_len = ft_strlen(tmp[i].key);
		tmp[i].value = ft_strdup("nullvalue");
		tmp[i].value_len = 0;
		tmp[i + 1].key = NULL;
		tmp[i + 1].value = NULL;
		ft_memdel(all->env_vars);
		all->env_vars = tmp;
		all->env_counter++;
	}
}
