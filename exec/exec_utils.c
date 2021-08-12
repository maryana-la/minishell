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

void	dup2_if_redirect(t_all *all)
{
	if (all->cmnd[all->i].fd_in > STDIN_FILENO)
	{
		dup2(all->cmnd[all->i].fd_in, STDIN_FILENO);
		close(all->cmnd[all->i].fd_in);
	}
	if (all->cmnd[all->i].fd_out > STDOUT_FILENO)
	{
		dup2(all->cmnd[all->i].fd_out, STDOUT_FILENO);
		close(all->cmnd[all->i].fd_out);
	}
}

void	replace_fds_child(t_all *all)
{
	if (all->cmnd[all->i].fd_in < 0 || all->cmnd[all->i].fd_out < 0)
		exit (1);
	if (all->cmnd[all->i].fd_in > STDIN_FILENO)
	{
		dup2(all->cmnd[all->i].fd_in, STDIN_FILENO);
		close(all->cmnd[all->i].fd_in);
	}
	else if (all->i != 0)
	{
		dup2(all->fd_tmp, STDIN_FILENO);
		close (all->fd_tmp);
	}
	close(all->fd[0]);
	if (all->cmnd[all->i].fd_out > STDOUT_FILENO)
	{
		dup2(all->cmnd[all->i].fd_out, 1);
		close(all->cmnd[all->i].fd_out);
	}
	else if (all->i != all->pip_count)
		dup2(all->fd[1], 1);
	close(all->fd[1]);
}

void	handle_fds_parent(t_all *all)
{
	close(all->fd[1]);
	if (all->i != 0)
		close(all->fd_tmp);
	all->fd_tmp = dup(all->fd[0]);
	close(all->fd[0]);
}

void	fork_error(t_all *all, pid_t *pid)
{
	int	i;

	i = 0;
	while (i <= all->i)
	{
		kill(pid[i], SIGTERM);
		i++;
	}
	exec_error_print("fork", "Resource temporarily unavailable");
	g_status_exit_code = 1;
}

void	wait_children(t_all *all, pid_t *pid)
{
	int	wstat;
	int	i;

	i = -1;
	while (++i < all->pip_count + 1)
	{
		waitpid(pid[i], &wstat, 0);
		get_exit_status(wstat);
	}
}
