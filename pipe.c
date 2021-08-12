#include "minishell.h"

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

void	start_commands_pipes(t_all *all)
{
	pid_t	*pid;

	pid = malloc(sizeof(pid_t) * (all->num_of_pipes + 1));
	while (all->i < all->pip_count + 1)
	{
		pipe(all->fd);
		pid[all->i] = fork();
		if (pid[all->i] == -1 || all->i > 450)
		{
			fork_error(all, pid);
			break ;
		}
		else if (pid[all->i] == 0)
		{
			replace_fds_child(all);
			start_commands(all);
			exit(g_status_exit_code);
		}
		handle_fds_parent(all);
		all->i++;
	}
	wait_children(all, pid);
	ft_memdel(pid);
}

void	launch_commands(t_all *all)
{
	all->i = 0;
	g_status_exit_code = 0;
	if (all->pip_count == 0)
	{
		if (all->cmnd[all->i].fd_in < 0 || all->cmnd[all->i].fd_out < 0)
		{
			g_status_exit_code = 1;
			return ;
		}
		start_commands(all);
	}
	else
		start_commands_pipes(all);
	dup2(all->fd_std[0], STDIN_FILENO);
	dup2(all->fd_std[1], STDOUT_FILENO);
}
