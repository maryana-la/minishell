#include "minishell.h"

int g_status_exit_code = 0;

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
//		arg = check_lower_case(arg);
		all->cmnd[all->pip_count].args = malloc(sizeof(char *) * 2);
		all->cmnd[all->pip_count].args[0] = ft_strdup(arg);
		all->cmnd[all->pip_count].args[1] = NULL;
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
//			ft_memdel(all->cmnd[all->pip_count].args[i]);
		}

		tmp[i] = ft_strdup(arg);
		tmp[i + 1] = NULL;
		ft_memdel_double(all->cmnd[all->pip_count].args);
		all->cmnd[all->pip_count].args = tmp;
	}
}

void find_next_cmnd(char *str, int *i)
{
	while (str[*i] && str[*i] != '|')
		(*i)++;
}

void ft_parser(char *str, t_all *all)
{
	int i;
	int j;
	int len;
	char *tmp;
	char *old_str;

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
	    {
	    	if(ft_handle_redirect(str, &i, all))
	    		find_next_cmnd(str, &i);
	    }
	    else if (str[i] == '|')
	    {
	        all->pip_count++;
	        i++;
	    }
	    else
	    {
			old_str = str;
	    	str = replace_env_with_value(str, i, all); // заменяем в строке переменные окружения
	    	ft_memdel(old_str);
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
	        ft_memdel(tmp);
	    }
	}
	ft_memdel(str);
	if (all->cmnd->args)
		launch_commands(all);
}

void 	ft_free_commands(t_all *all)
{
	int i;

	i = 0;
	if (all->cmnd)
	{
		while(all->cmnd[i].args)
		{
			ft_memdel_double(all->cmnd[i].args);
			i++;
		}
		free(all->cmnd);
		all->cmnd = NULL;
	}
}

void init_all(t_all *all)
{
	ft_free_commands(all);
	all->num_of_pipes = 0;
}

int take_input(t_all *all, char** str)
{
	char* buf;

	rl_catch_signals = 0;
	buf = readline(GREEN "minishell> " RESET);
	if (!buf)
		print_and_exit(all, 0);
	if (ft_strlen(buf) != 0)
	{
		add_history(buf);
		*str = ft_strdup(buf);
		buf = ft_memdel(buf);
		return 0;
	}
	else
	{
		buf = ft_memdel(buf);
		return 1;
	}
}

int main(int argc, char **argv, char **env)
{
	t_all  all;

	rl_outstream = stderr;//todo не забыть убрать перед сдачей
	(void)argc;
	(void)argv;
	all.cmnd = NULL;
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
		if (take_input(&all, &str))
			continue;
		if (ft_preparser(str, &all) == 0)
			ft_parser(str, &all);
		else
			ft_memdel(str);
		init_all(&all);
	}
	return 0;
}


