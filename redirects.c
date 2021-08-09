#include "minishell.h"

char	*get_file_name(char *str, int *i, int type, t_all *all)
{
	int	len;
	char	*tmp;
	char	*str_origin;
	int j;
	int x;

	skip_spaces(str, i);

	str_origin = ft_strdup(str);
	x = *i; //чтобы в конце вернуть указатель после имени файла
	while (str_origin[x] && !(check_set(str_origin[x], " \t|;<>")))
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
		str = replace_env_with_value(str, j, all);
	len = get_arg_len(str, *i);
	if (!len)
		return NULL;
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL); //malloc error
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
	*i = x;
	return(tmp);
}

void	heredoc_stdin_read(t_all *all, char *stop)
{
	char	*line;
	char 	*heredoc_file;
	int		ret;

//	heredoc_file = ft_strjoin(stop, "tmp_file");
	heredoc_file = ft_strdup("tmp_file_heredoc");
	all->cmnd[all->pip_count].fd_in = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (all->cmnd[all->pip_count].fd_in < 0 || read(all->cmnd[all->pip_count].fd_in, 0, 0) < 0)
	{
		all->last_exit = errno;
		printf("minishell: %s: %s\n", heredoc_file, strerror(errno));
		unlink(heredoc_file);
		ft_memdel(heredoc_file);
		return ;
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
		all->last_exit = errno;
		printf("minishell: %s: %s\n", heredoc_file, strerror(errno));
		unlink(heredoc_file);
		ft_memdel(heredoc_file);
		return ;
	}
	unlink(heredoc_file);
	ft_memdel(heredoc_file);
}


void	ft_handle_redirect(char *str, int *i, t_all *all)
{
	char *file_name;
	char *stop_word;

	if (str[*i] == '>' && str[*i + 1] != '>')
	{
		(*i)++;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			all->last_exit = 1;
			printf("minishell: ambiguous redirect\n");
			all->cmnd[all->pip_count].fd_out = -2;
			return ;
		}
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
		if (all->cmnd[all->pip_count].fd_out < 0 || read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
		{
			all->last_exit = errno;
			printf("minishell: %s: %s\n", file_name, strerror(errno));
			ft_memdel(file_name);
			return ;//todo maybe change function to int?
		}

	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			all->last_exit = 1;
			printf("minishell: ambiguous redirect\n");
			all->cmnd[all->pip_count].fd_out = -2;
			return ;
		}
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
		if (all->cmnd[all->pip_count].fd_out < 0 || read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
		{
			all->last_exit = errno;
			printf("minishell: %s: %s\n", file_name, strerror(errno));
			ft_memdel(file_name);
			return ;//todo maybe change function to int?
		}
	}
	else if (str[*i] == '<' && str[*i + 1] != '<')
	{
		(*i)++;
		file_name = get_file_name(str, i, 1, all);
		if (!file_name)
		{
			all->last_exit = 1;
			printf("minishell: ambiguous redirect\n");
			all->cmnd[all->pip_count].fd_in = -2;
			return ;
		}
		all->cmnd[all->pip_count].fd_in = open(file_name, O_RDONLY);
		if (all->cmnd[all->pip_count].fd_in < 0 || read(all->cmnd[all->pip_count].fd_in, NULL, 0) < 0)
		{
			all->last_exit = errno;
			printf("minishell: %s: %s\n", file_name, strerror(errno));
			ft_memdel(file_name);
			return ;//todo maybe change function to int?
		}
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*i = *i + 2;
		stop_word = get_file_name(str, i, 2, all);
		heredoc_stdin_read(all, stop_word);
		ft_memdel(stop_word);
	}
}