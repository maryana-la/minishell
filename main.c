#include "minishell.h"

char *ft_slash(char *str, int *i)
{
	char *tmp;
	char *tmp2;

	tmp = ft_substr(str, 0, *i);
	tmp2 = ft_strdup(str + *i + 1);
	tmp = ft_strjoin(tmp, tmp2);
	*i = *i + 2;
	return(tmp);
}

int check_set(char c, char *set)
{
	int i;

	i = -1;
	while (set[++i] != '\0')
		if (set[i] == c)
			return (1);
	return 0;
}

void skip_spaces(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
}

char *ft_dollar(char *str, int *i, t_all *all)
{
	char	*var;
	int		pos_of_dollar;
	int		end_of_var;
	char	*tmp1;
	char	*tmp2;
	char	*value;
/*
**todo Maryana добавить обработку $ $1 $12 $$  $?
*/
	pos_of_dollar = *i;
	while(str[++*i]) //find end of variable
		if(check_set(str[*i], " \t\'\"\\$;|><"))
			break;
	end_of_var = *i;

	if (end_of_var - pos_of_dollar == 1)
		return(str);

//cut variable
	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar -1));
	// printf("var = %s\n", var);

	if (ft_isdigit(var[0]) != 0) // check all ascii symbols
	{
		if(ft_strlen(var) == 1)
		{
			tmp1 = ft_substr(str, 0, pos_of_dollar);
			tmp2 = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
			tmp1 = ft_strjoin(tmp1, tmp2);
			return (tmp1);
		}
		else
		{
			tmp1 = ft_substr(str, 0, pos_of_dollar);
			tmp2 = ft_substr(str, pos_of_dollar + 2, (ft_strlen(str) - pos_of_dollar - 1));
			tmp1 = ft_strjoin(tmp1, tmp2);
			return (tmp1);
		}
	}

//find variable in the lists

	int j = -1;
	while (all->env_vars[++j].key) //find variable in the lists
		if (ft_strncmp(all->env_vars[j].key, var, (ft_strlen(var) + 1)) == 0)
			break;
	if (all->env_vars[j].key) //если долистал до конца или вылетел из цикла
		value = ft_strdup(all->env_vars[j].value);
	else
		value = "\0";
	tmp1 = ft_substr(str, 0, pos_of_dollar); // cut till $
	tmp2 = ft_substr(str, *i, (ft_strlen(str) - *i + 1)); // cut after variable
	tmp1 = ft_strjoin(tmp1, value);
	tmp1 = ft_strjoin(tmp1, tmp2);
	*i = pos_of_dollar + ft_strlen(value) - 1;
	return (tmp1);
}

char	*replace_env_with_value(char *str, t_all *all)
{
	int flag;
	int i;

	flag = 0;
	i = -1;
// заменяем переменные только до следующей команды
	while(str[++i] && !(check_set(str[i], "|;><")))
	{
		if(str[i] == '\'')
		{
			if(flag == 0)
				flag = 1;
			else
				flag = 0;
		}
			// else if (str[i] == '\\' && flag == 0)
			//  	str = ft_slash(str, &i);
		else if(str[i] == '$' && str[i - 1] != '\\' && flag == 0)
			str = ft_dollar(str, &i, all);
	}
	return(str);
}

int get_arg_len(char *str, int i)
{
	int len;
	int pos;

	pos = i;
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

char	*check_lower_case(char *str)
{
	int i;
	char *dest;

	i = 0;
	dest = malloc(sizeof(char) * ft_strlen(str));
	while (str[i] != 0)
	{
		dest[i] = ft_tolower(str[i]);
		i++;
	}
	if (!ft_strncmp(dest, "echo", 5) || !ft_strncmp(dest, "pwd", 4) || !ft_strncmp(dest, "env", 4))
	{
		free(str);
		return(dest);
	}
	free(dest);
	return(str);
}

void	ft_put_str_to_struct(char *arg, t_all *all)
{
	int i;
	char **tmp;

	i = 0;

	if (!all->cmnd[all->pip_count].args)
	{
		arg = check_lower_case(arg);
		all->cmnd[all->pip_count].args = malloc(sizeof(char *) * 2);
		all->cmnd[all->pip_count].args[0] = arg;
		all->cmnd[all->pip_count].args[1] = 0;
	}
	else
	{
		i=0;
		while (all->cmnd[all->pip_count].args[i] != NULL)
			i++;
		tmp = malloc(sizeof(char *) * (i + 2));
		i = 0;
		while (all->cmnd[all->pip_count].args[i] != 0)
		{
			tmp[i] = ft_strdup(all->cmnd[all->pip_count].args[i]);
			i++;
		}
		tmp[i] = ft_strdup(arg);
		tmp[i + 1] = 0;

		free(all->cmnd[all->pip_count].args);
		if (arg) {
			free(arg);
			arg = 0;
		}
		all->cmnd[all->pip_count].args = tmp;
	}
}

char	*get_file_name(char *str, int *i, t_all *all)
{
	int	len;
	char *tmp;
	int j;

	skip_spaces(str, i);
	len = get_arg_len(str, *i);
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (0); //malloc error
	j = 0;
	while (str[*i] && !check_set(str[*i], " \t|;<>"))
	{
		if (str[*i] == '\'')
		{
			while (str[++(*i)] && str[*i] != '\'')
			{
				tmp[j] = str[(*i)];
				j++;
			}
			j--;
		}
		else if (str[*i] == '\"')
		{
			while (str[++(*i)] && str[*i] != '\"')
			{
				if (str[*i] == '\\' && (str[*i + 1] == '$' || str[*i + 1] == '\'' || str[*i + 1] == '\"' || str[*i + 1] == '\\'))
					tmp[j] = str[++(*i)];
				else
					tmp[j] = str[*i];
				j++;
			}
			j--;
		}
		else if (str[*i] == '\\')
			tmp[j] = str[++(*i)];
		else
			tmp[j] = str[*i];
		(*i)++;
		j++;
	}
	tmp[j] = '\0';
	return(tmp);
}

void	heredoc_stdin_read(t_all *all, char *stop)
{
	char	*line;
	int		ret;

	all->cmnd[all->pip_count].fd_in = open("tmp_file", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
//	if (pip->fd_in < 0 || read(pip->fd_in, 0, 0) < 0)
//		ft_error_exit(argv[1], pip, FILE_ERR);
	ret = 1;
	while (ret)
	{
		write(0, "> ", 2);
		ret = get_next_line(0, &line);
		if (ft_strncmp(line, stop, ft_strlen(stop) + 1) == 0)
			break ;
		write(all->cmnd[all->pip_count].fd_in, line, ft_strlen(line));
		write(all->cmnd[all->pip_count].fd_in, "\n", 1);
		if (line)
			free(line);
	}
	if (line)
		free(line);


	close (all->cmnd[all->pip_count].fd_in);
	all->cmnd[all->pip_count].fd_in = open("tmp_file", O_RDONLY);
//	if (all->cmnd->fd_in < 0 || read(all->cmnd->fd_in, 0, 0) < 0)
//		ft_error_exit("tmp_file", pip, FILE_ERR);
}


void	ft_handle_redirect(char *str, int *i, t_all *all)
{
	char *file_name;

	if (str[*i] == '>' && str[*i + 1] != '>')
	{
		(*i)++;
		file_name = get_file_name(str, i, all);
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, all);
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	}
	else if (str[*i] == '<' && str[*i + 1] != '<')
	{
		(*i)++;
		file_name = get_file_name(str, i, all);
		all->cmnd[all->pip_count].fd_in = open(file_name, O_RDONLY);
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, all);
		heredoc_stdin_read(all, file_name);
	}
}

void ft_parser(char *str, t_all *all)
{
	int i;
	int j;
	int len;
	char *tmp;
	char *from_quote;


	all->cmnd = malloc(sizeof(t_cmnd) * (all->num_of_pipes + 2)); //todo change to number of pipes
	i = -1;
	while (++i < (all->num_of_pipes + 2))
	{
		all->cmnd[i].args = NULL;
		all->cmnd[i].fd_in = -1;
		all->cmnd[i].fd_out = -1;
	}
	all->pip_count = 0;
	str = replace_env_with_value(str, all); // заменяем в строке переменные окружения
	i = 0;
//todo malloc for cmnd array

	while(str[i])
	{
//		while (str[i] && !(check_set(str[i], "|;")))
//		{
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
				len = get_arg_len(str, i);
				tmp = malloc(sizeof(char) * (len + 1));
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
					} else if (str[i] == '\"')
					{
						while (str[++i] && str[i] != '\"')
						{
							if (str[i] == '\\' &&
								(str[i + 1] == '$' || str[i + 1] == '\'' || str[i + 1] == '\"' || str[i + 1] == '\\'))
								tmp[j] = str[++i];
							else
								tmp[j] = str[i];
							j++;
						}
						j--;
					} else if (str[i] == '\\')
						tmp[j] = str[++i];
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
			}
//		}
	}
	launch_commands(all);
}


void env_init(t_all *all, char **env) // env init with lists:
{
	t_env	*tmp;
	int shlvl_tmp = 0;
	int pwd_flag = 0;
	int oldpwd_flag = 0;
	int shlvl_flag = 0;

	shlvl_tmp = 0;

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
			all->env_vars[i].value = getcwd(NULL, -1);
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
			all->env_vars[i].value = ft_substr(env[i], 6, (ft_strlen(env[i]) - 6));
			shlvl_tmp = ft_atoi(all->env_vars[i].value) + 1;
			all->env_vars[i].value = ft_itoa(shlvl_tmp);
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
		free(all->env_vars);
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
		tmp[i].value = getcwd(NULL, -1);
		tmp[i].value_len = ft_strlen(tmp[i].value);
		tmp[i + 1].key = NULL;
		tmp[i + 1].value = NULL;
		free(all->env_vars);
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
		free(all->env_vars);
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
		while (j < all->num_of_pipes + 1)
		{
			i = -1;
			if (all->cmnd[j].args[++i])
			{
				while (all->cmnd[j].args[i])
				{
					if (all->cmnd[j].args[i])
						all->cmnd[j].args[i] = NULL;
				}
//				all->cmnd[j].args[i] = NULL;
			}
			j++;
		}
	}
	all->cmnd = NULL;
	all->num_of_pipes = 0;
}

int takeInput(t_all *all, char** str)
{
	char* buf;

	rl_catch_signals = 0;
	buf = readline("minishell > ");
	if (!buf)
		exit_command(all);
	if (strlen(buf) != 0)
	{
		add_history(buf);
		*str = ft_strdup(buf);
		if (buf)
			free(buf);
		return 0;
	}
	else
		return 1;
}

int main(int argc, char **argv, char **env)
{
	t_all  all;
	int i = 0;
	char **test;

	init_all(&all);
	env_init(&all, env);

//	char *str = "ECHO $SHLVL'pwd $PATH' \"$PAGER$LSCOLORS\"$;l$XPC_FLAGS\'ffrsvdd\'";

if (signal(SIGINT, sig_handler) == SIG_ERR)
		error_handler(&all, 3);

	char *str;
	while (1)
	{
		init_all(&all);
		if (takeInput(&all, &str))
			continue;
		//	printf("str_i = %s\n", str);
		if (ft_preparser(str, &all) > 0)
			ft_parser(str, &all);
		else
			printf("%s\n", "preparser error");

		if (str) {
			free(str);
			str = NULL;
		}
	}
	return 0;
}


