#include "minishell.h"

int	check_set(char c, char *set)
{
	int i;

	i = -1;
	while (set[++i] != '\0')
		if (set[i] == c)
			return (1);
	return 0;
}

void	skip_spaces(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
}

char	*ft_dollar(char *str, int *i, t_all *all) //done leaks
{
	char	*var;
	int		pos_of_dollar;
	int		end_of_var;
	char	*begin_of_str;
	char	*end_of_line;
	char	*tmp;
	char	*value;

	pos_of_dollar = *i;
	while(str[++*i]) //find end of variable
		if(check_set(str[*i], " \t\'\"\\$;|></="))
			break;
	end_of_var = *i;

	if (end_of_var - pos_of_dollar == 1)
		return(str);
//cut variable
	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar -1));

	if (!(ft_strncmp(var, "?", 1)))
	{
		value = ft_itoa(g_status);
		begin_of_str = ft_substr(str, 0, pos_of_dollar);
		end_of_line = ft_substr(str, (pos_of_dollar + 2), (ft_strlen(str) - pos_of_dollar - 1));
		tmp = ft_strjoin(begin_of_str, value);
//		ft_memdel(begin_of_str);
//		begin_of_str = str; //to make free later
		str = ft_strjoin(tmp, end_of_line);
		*i = pos_of_dollar + (int)ft_strlen(value) - 1;
		ft_memdel(tmp);
		ft_memdel(begin_of_str);
		ft_memdel(value);
		ft_memdel(end_of_line);
		ft_memdel(var);
		return(str);
	}
	else if (ft_isdigit(var[0]) != 0) // if numeric
	{
		if(ft_strlen(var) == 1)
			end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
		else
			end_of_line = ft_substr(str, pos_of_dollar + 2, (ft_strlen(str) - pos_of_dollar - 1));
		begin_of_str = ft_substr(str, 0, pos_of_dollar);
//		tmp = str;
		str = ft_strjoin(begin_of_str, end_of_line);
		ft_memdel(begin_of_str);
		ft_memdel(end_of_line);
		ft_memdel(var);
//		ft_memdel(tmp);
		return (str);
	}

//find variable in the lists

	int j = -1;
	while (all->env_vars[++j].key) //find variable in the lists
		if (ft_strncmp(all->env_vars[j].key, var, (ft_strlen(var) + 1)) == 0)
			break;
	if (all->env_vars[j].key) //если долистал до конца или вылетел из цикла
		value = ft_strdup(all->env_vars[j].value);
	else
		value = ft_strdup("\0");
	begin_of_str = ft_substr(str, 0, pos_of_dollar); // cut till $
	end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1)); // cut after variable
	tmp = ft_strjoin(begin_of_str, value);
//	ft_memdel(begin_of_str);
//	begin_of_str = str; //to make free later
	str = ft_strjoin(tmp, end_of_line);
	*i = pos_of_dollar + (int)ft_strlen(value) - 1;
	ft_memdel(tmp);
	ft_memdel(begin_of_str);
	ft_memdel(value);
	ft_memdel(end_of_line);
	ft_memdel(var);
	return (str);
}

char	*replace_env_with_value(char *str, int i, t_all *all)
{
	int flag;

	flag = 0;

// заменяем переменные только до следующей команды
	while(str[i] && !(check_set(str[i], "|><")))
	{
		if(str[i] == '\'')
		{
			if(flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		else if(str[i] == '$' && flag == 0)
			str = ft_dollar(str, &i, all);
		i++;
	}
	return(str);
}

int	get_arg_len(char *str, int i) //no malloc
{
	int len;

	len = 0;
	while(str[i] && !check_set(str[i], " \t|;<>"))
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
			{
				len++;
				i++;
			}
			i++;
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\"')
			{
				len++;
				i++;
			}
			i++;
		}
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*check_lower_case(char *str) //done leaks
{
	int i;
	char *dest;

	i = 0;
	dest = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dest)
		return (NULL);
	while (str[i] != 0)
	{
		dest[i] = ft_tolower(str[i]);
		i++;
	}
	dest[i] = '\0';
	ft_memdel(str);
	return(dest);
}

void	ft_put_str_to_struct(char *arg, t_all *all) //done leaks
{
	int i;
	char **tmp;

	if (!all->cmnd[all->pip_count].args) //if first argument
	{
		arg = check_lower_case(arg);
		all->cmnd[all->pip_count].args = malloc(sizeof(char *) * 2);
		all->cmnd[all->pip_count].args[0] = ft_strdup(arg);
		all->cmnd[all->pip_count].args[1] = NULL; //todo maybe change to NULL?
		ft_memdel(arg);
	}
	else
	{
		i = 0;
		while (all->cmnd[all->pip_count].args[i] != NULL)
			i++;
		tmp = malloc(sizeof(char *) * (i + 2));
		i = -1;
		while (all->cmnd[all->pip_count].args[++i] != 0) //copy all to a tmp massive and free previous commands
		{
			tmp[i] = ft_strdup(all->cmnd[all->pip_count].args[i]);
			ft_memdel(all->cmnd[all->pip_count].args[i]);
		}

		tmp[i] = ft_strdup(arg);
		tmp[i + 1] = NULL; //maybe change to NULL?
		ft_memdel(arg);
		all->cmnd[all->pip_count].args = tmp;
	}
}

void ft_parser(char *str, t_all *all)
{
	int i;
	int j;
	int len;
	char *tmp;

	all->cmnd = malloc(sizeof(t_cmnd) * (all->num_of_pipes + 2)); //malloc for number of commands
	i = -1;
	while (++i < (all->num_of_pipes + 2))
	{
		all->cmnd[i].args = NULL;
		all->cmnd[i].fd_in = STDIN_FILENO;
		all->cmnd[i].fd_out = STDOUT_FILENO;
	}
	all->pip_count = 0;
	i = 0;

	while(str[i])
	{
	    skip_spaces(str, &i);
	    if (str[i] == '>' || str[i] == '<')
	        ft_handle_redirect(str, &i, all);
	    else if (str[i] == '|')
	    {
	        all->pip_count++;
	        i++;
	    }
	    else
	    {
			str = replace_env_with_value(str, i, all); // заменяем в строке переменные окружения
	    	len = get_arg_len(str, i);
	        tmp = ft_calloc((len + 2), sizeof(char));
	        j = 0;
	        while (str[i] && !check_set(str[i], " \t|;<>"))
	        {
	            if (str[i] == '\'')
	            {
	                while (str[++i] && str[i] != '\'')
	                {
	                    tmp[j] = str[i];
	                    j++;
	                }
	                j--;
	            }
	            else if (str[i] == '\"')
	            {
	                while (str[++i] && str[i] != '\"')
	                {
						tmp[j] = str[i];
	                    j++;
	                }
	                j--;
	            }
	            else
	                tmp[j] = str[i];
	            i++;
	            j++;
	        }
	        if (len > 0)
	        {
	            tmp[j] = '\0';
	            ft_put_str_to_struct(tmp, all);
	        }
//	        ft_memdel(tmp);
	    }
	}
	if (all->cmnd->args)
		launch_commands(all);
}


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
		tmp[i].key = "SHLVL";
		tmp[i].value = "1";
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

void init_all(t_all *all)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	if (all->cmnd)
	{
//		while (j < all->num_of_pipes + 1)
//		{
//			i = 0;
//			if (all->cmnd[j].args[i])
//			{
//				while (all->cmnd[j].args[i])
//				{
//					if (all->cmnd[j].args[i])
//						all->cmnd[j].args[i] = NULL;
//					i++;
//				}
////				all->cmnd[j].args[i] = NULL;
//			}
//			j++;
//		}
		all->cmnd = NULL;
	}
//	all->cmnd = NULL;
	all->num_of_pipes = 0;
}

int take_input(t_all *all, char** str)
{
	char* buf;

	rl_catch_signals = 0;
	buf = readline("minishell> ");
	if (!buf)
		print_and_exit(all, 0);
	if (ft_strlen(buf) != 0)
	{
		add_history(buf);
		*str = ft_strdup(buf);
		ft_memdel(buf);
		return 0;
	}
	else
		return 1;
}

int main(int argc, char **argv, char **env)
{
	t_all  all;

	rl_outstream = stderr;//todo не забыть убрать перед сдачей
	(void)argc;
	(void)argv;
	init_all(&all);
	env_init(&all, env);
	all.fd_std[0] = dup(0);
	all.fd_std[1] = dup(1);

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("Signal init error\n");
	if (signal(SIGQUIT, sig_handler) == SIG_ERR)
		printf("Signal init error\n");


	char *str;
	while (1)
	{
		init_all(&all);
		if (take_input(&all, &str))
			continue;
		if (ft_preparser(str, &all) == 0)
			ft_parser(str, &all);
		ft_memdel(str);
	}
	return 0;
}


