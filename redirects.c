#include "minishell.h"

char	*get_file_name(char *str, int *i, int type, t_all *all)
{
	int	len;
	char	*tmp;
	int j;
	int x;
	char *str_new;

	skip_spaces(str, i);

	x = *i; //чтобы в конце вернуть указатель после имени файла
	while (str[x] && !(check_set(str[x], " \t|;<>")))
	{
		if (str[x] == '\'')
		{
			x++;
			while (str[x] != '\'')
				x++;
			x++;
		}
		else if (str[x] == '\"')
		{
			x++;
			while (str[x] != '\"')
				x++;
			x++;
		}
		else
			x++;
	}



	if (type != 2)
		str_new = replace_env_with_value(str, *i, all);//todo keep original function
	else
		str_new = ft_strdup(str);
	len = get_arg_len(str_new, *i);
	if (!len)
	{
		*i = x;
		ft_memdel(str_new);
		return NULL;
	}
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
	{
		*i = x;
		ft_memdel(str_new);
		return (NULL); //malloc error
	}
	j = 0;
	while (str_new[*i] && !check_set(str_new[*i], " \t|;<>"))
	{
		if (str_new[*i] == '\'')
		{
			while (str_new[++(*i)] && str_new[*i] != '\'')
			{
				tmp[j] = str_new[(*i)];
				j++;
			}
			j--;
		}
		else if (str_new[*i] == '\"')
		{
			while (str_new[++(*i)] && str_new[*i] != '\"')
			{
				if (str_new[*i] == '\\' && (str_new[*i + 1] == '$' || str_new[*i + 1] == '\'' || str_new[*i + 1] == '\"' || str_new[*i + 1] == '\\'))
					tmp[j] = str_new[++(*i)];
				else
					tmp[j] = str_new[*i];
				j++;
			}
			j--;
		}
		else if (str_new[*i] == '\\')
			tmp[j] = str_new[++(*i)];
		else
			tmp[j] = str_new[*i];
		(*i)++;
		j++;
	}
	tmp[j] = '\0';
	ft_memdel(str_new);
	*i = x;
	return(tmp);
}

int	heredoc_stdin_read(t_all *all, char *stop)
{
	char	*line;
	char 	*heredoc_file;
	int		ret;

	heredoc_file = ft_strdup("tmp_file_heredoc");
	all->cmnd[all->pip_count].fd_in = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (all->cmnd[all->pip_count].fd_in < 0 || read(all->cmnd[all->pip_count].fd_in, 0, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(heredoc_file, strerror(errno));
		unlink(heredoc_file);
		ft_memdel(heredoc_file);
		return (1);
	}
	ret = 1;
	while (ret)
	{
		write(0, "> ", 2);
		ret = get_next_line(0, &line);
		if (ft_strncmp(line, stop, ft_strlen(stop) + 1) == 0)
			break ;
		write(all->cmnd[all->pip_count].fd_in, line, ft_strlen(line));
		write(all->cmnd[all->pip_count].fd_in, "\n", 1);
		ft_memdel(line);
	}
	ft_memdel(line);

	close (all->cmnd[all->pip_count].fd_in);
	all->cmnd[all->pip_count].fd_in = open(heredoc_file, O_RDONLY);
	if (all->cmnd[all->pip_count].fd_in < 0 || read(all->cmnd[all->pip_count].fd_in, 0, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(heredoc_file, strerror(errno));
		unlink(heredoc_file);
		ft_memdel(heredoc_file);
		return (1);
	}
	unlink(heredoc_file);
	ft_memdel(heredoc_file);
	return (0);
}


int	ft_handle_redirect(char *str, int *i, t_all *all)
{
	char *file_name;

	if (str[*i] == '>' && str[*i + 1] != '>')
	{
		(*i)++;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			g_status_exit_code = 1;
			exec_error_print(" ", "ambiguous redirect");
			all->cmnd[all->pip_count].fd_out = -2;
			return (1);
		}
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
		if (all->cmnd[all->pip_count].fd_out < 0 || read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
		{
			g_status_exit_code = errno;
			exec_error_print(file_name, strerror(errno));
			ft_memdel(file_name);
			return (1);
		}

	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			g_status_exit_code = 1;
			exec_error_print(" ", "ambiguous redirect");
			all->cmnd[all->pip_count].fd_out = -2;
			return (1);
		}
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
		if (all->cmnd[all->pip_count].fd_out < 0 || read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
		{
			g_status_exit_code = errno;
			exec_error_print(file_name, strerror(errno));
			ft_memdel(file_name);
			return (1);
		}
	}
	else if (str[*i] == '<' && str[*i + 1] != '<')
	{
		(*i)++;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			g_status_exit_code = 1;
			exec_error_print(" ", "ambiguous redirect");
			all->cmnd[all->pip_count].fd_in = -2;
			return (1);
		}
		all->cmnd[all->pip_count].fd_in = open(file_name, O_RDONLY);
		if (all->cmnd[all->pip_count].fd_in < 0 || read(all->cmnd[all->pip_count].fd_in, NULL, 0) < 0)
		{
			g_status_exit_code = errno;
			exec_error_print(file_name, strerror(errno));
			ft_memdel(file_name);
			return (1);
		}
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, 2, all);
		if (heredoc_stdin_read(all, file_name))
		{
			ft_memdel(file_name);
			return (1);
		}
	}
	ft_memdel(file_name);
	return(0);
}