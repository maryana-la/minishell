#include "minishell.h"

void 	launch_commands(t_all *all)
{
	pid_t *pid;

	all->i = 0;
	g_status = 0;

	if (all->pip_count == 0)
	{
		if (all->cmnd[all->i].fd_in < 0 || all->cmnd[all->i].fd_out < 0)
		{
			g_status = 1;
			return ;
		}
		start_commands(all);
	}

	else
	{
		all->i = 0;
		pid = malloc(sizeof(pid_t) * (all->num_of_pipes + 1));
		while (all->i < all->pip_count + 1)
		{
			pipe(all->fd); //new
			pid[all->i] = fork();
			if (pid[all->i] == -1 || all->i > 300)
			{
				int i = 0;
				while (i <= all->i)
				{
					kill(pid[i], SIGTERM);
					i++;
				}
//
//				i = 0;
//				while (i <= all->i)
//				{
//					kill(pid[i], SIGKILL);
//					i++;
//				}
				exec_error_print("fork", "Resource temporarily unavailable");
				g_status = 1;
				break ;
			}
			else if (pid[all->i] == 0) //child starts here
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


				if (!ft_strncmp(all->cmnd[all->i].args[0], "pwd", 4))
					pwd_command(all);
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "env", 4))
					print_env_list(all->env_vars, 0, all->env_counter);
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "export", 7))
					export_command(all);
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "unset", 6))
					unset_command(all);
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "cd", 3))
					(cd_command(all));
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "echo", 5))
					echo_command(all);
				else if (!ft_strncmp(all->cmnd[all->i].args[0], "exit", 5))
					exit_command(all);
				else
					cmd_exec1(all);
				exit(g_status);
			}
			close(all->fd[1]);
			if (all->i != 0)
				close(all->fd_tmp);
			all->fd_tmp = dup(all->fd[0]);
			close(all->fd[0]);
			all->i++;
		}

		int wstat;
		int i = -1;
		while (++i < all->pip_count + 1)
		{
			waitpid(pid[i], &wstat, 0);
			if (WIFEXITED(wstat))
			{
				int exit_code = WEXITSTATUS(wstat);
				if (exit_code != 0)
				{
					if (exit_code == 13)
						g_status = 126;
					else if (exit_code == 14)
						g_status = 127;
					else
						g_status = exit_code;
				}
				else
					g_status = 0;
			}
			else
				g_status = 0;
		}
		ft_memdel(pid);
	}
	dup2(all->fd_std[0], 0);
	dup2(all->fd_std[1], 1);
}