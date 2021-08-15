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

int	ft_handle_redirect(char *str, int *i, t_all *all)
{
	if (str[*i] == '>' && str[*i + 1] != '>')
		return (redirect_out_1(all, i, str));
	else if (str[*i] == '>' && str[*i + 1] == '>')
		return (redirect_out_2(all, i, str));
	else if (str[*i] == '<' && str[*i + 1] != '<')
		return (redirect_in_1(all, i, str));
	else if (str[*i] == '<' && str[*i + 1] == '<')
		return (redirect_in_2(all, i, str));
	return (0);
}

int	redirect_out_1(t_all *all, int *i, char *str)
{
	char	*file_name;

	file_name = NULL;
	(*i)++;
	file_name = get_file_name(str, i, 1, all);
	if (!file_name)
	{
		g_status_exit_code = 1;
		exec_error_print(" ", "ambiguous redirect");
		all->cmnd[all->pip_count].fd_out = -2;
		return (1);
	}
	all->cmnd[all->pip_count].fd_out = \
		open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (all->cmnd[all->pip_count].fd_out < 0 || \
		read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(file_name, strerror(errno));
		ft_memdel(file_name);
		return (1);
	}
	ft_memdel(file_name);
	return (0);
}

int	redirect_out_2(t_all *all, int *i, char *str)
{
	char	*file_name;

	file_name = NULL;
	*i = *i + 2;
	file_name = get_file_name(str, i, 1, all);
	if (!file_name)
	{
		g_status_exit_code = 1;
		exec_error_print(" ", "ambiguous redirect");
		all->cmnd[all->pip_count].fd_out = -2;
		return (1);
	}
	all->cmnd[all->pip_count].fd_out = \
		open(file_name, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
	if (all->cmnd[all->pip_count].fd_out < 0 || \
		read(all->cmnd[all->pip_count].fd_out, NULL, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(file_name, strerror(errno));
		ft_memdel(file_name);
		return (1);
	}
	ft_memdel(file_name);
	return (0);
}

int	redirect_in_1(t_all *all, int *i, char *str)
{
	char	*file_name;

	file_name = NULL;
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
	if (all->cmnd[all->pip_count].fd_in < 0 || \
		read(all->cmnd[all->pip_count].fd_in, NULL, 0) < 0)
	{
		g_status_exit_code = errno;
		exec_error_print(file_name, strerror(errno));
		ft_memdel(file_name);
		return (1);
	}
	ft_memdel(file_name);
	return (0);
}

int	redirect_in_2(t_all *all, int *i, char *str)
{
	char	*stop_word;

	stop_word = NULL;
	*i = *i + 2;
	stop_word = get_file_name(str, i, 2, all);
	if (heredoc_stdin_read(all, stop_word))
	{
		ft_memdel(stop_word);
		return (1);
	}
	ft_memdel(stop_word);
	return (0);
}
