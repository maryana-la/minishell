/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 23:20:33 by jjacquel          #+#    #+#             */
/*   Updated: 2021/07/07 23:39:40 by quadify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_open(t_all *all, char *heredoc_file)
{
	if (all->cmnd[all->pip_count].fd_in < 0 || \
	read(all->cmnd[all->pip_count].fd_in, 0, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(heredoc_file, strerror(errno));
		unlink(heredoc_file);
		ft_memdel(heredoc_file);
		return (1);
	}
	return (0);
}

void	read_stdin(t_all *all, char *stop)
{
	int		ret;
	char	*line;

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
}

int	heredoc_stdin_read(t_all *all, char *stop)
{
	char	*heredoc_file;

	heredoc_file = ft_strdup("tmp_file_heredoc");
	all->cmnd[all->pip_count].fd_in = \
		open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (check_open(all, heredoc_file))
		return (1);
	read_stdin(all, stop);
	close (all->cmnd[all->pip_count].fd_in);
	all->cmnd[all->pip_count].fd_in = open(heredoc_file, O_RDONLY);
	if (check_open(all, heredoc_file))
		return (1);
	unlink(heredoc_file);
	ft_memdel(heredoc_file);
	return (0);
}
