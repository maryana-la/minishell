#include "minishell.h"

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
		ft_memdel(line);
	}
	ft_memdel(line);


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
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i = *i + 2;
		file_name = get_file_name(str, i, all);
		all->cmnd[all->pip_count].fd_out = open(file_name, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
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