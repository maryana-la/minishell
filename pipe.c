//
// Created by Ragwyle Chelsea on 7/6/21.
//

#include "minishell.h"


void	call_child(t_all *all)
{
//	if (all->cmnd[all->i].fd_in > 0)
//	{
//		dup2(all->cmnd[all->i].fd_in, 0);
//		close(all->cmnd[all->i].fd_in);
//	}
//	else
//		dup2(fd_tmp, 0);
//	close(all->fd[0]);
//
//
//	if (all->cmnd[all->i].fd_out > 0)
//		dup2(all->cmnd[all->i].fd_out, 1);
//	else if (all->i != all->pip_count)
//		dup2(all->fd[1], 1);
//	close(all->fd[1]);

//	start_commands(all);


//	pip->path = get_data_path(argv[i], env, pip);
//	if (execve(pip->path, pip->arg_data, env) == -1)
//	{
//		ft_free_line(pip->path);
//		ft_free_array(pip->arg_data);
//		ft_putstr_fd(argv[i], 2);
//		ft_putstr_fd(" : command not found\n", 2);
//		exit (0);
//	}
//	ft_free_line(pip->path);
//	ft_free_array(pip->arg_data);
}


void 	launch_commands(t_all *all)
{
	char *path;
	pid_t pid;

	all->i = 0;
	all->fd_tmp = 0;
	while(all->i < all->pip_count + 1)
	{
		pipe(all->fd);
//		start_commands(all);
		if (!ft_strncmp(all->cmnd[all->i].args[0], "pwd", 4))
			pwd_command(all);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "env", 4))
			print_env_list(all->env_vars, 0, all->env_counter);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "export", 7))
			export_command(all);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "unset", 6))
			unset_command(all);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "cd", 3))
			cd_command(all);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "echo", 5))
			echo_command(all);
		else if (!ft_strncmp(all->cmnd[all->i].args[0], "exit", 5))
			exit_command(all);
		else
		{
			pid = fork();
			if (pid == -1)
				exit(-11);
			else if (pid == 0)
			{
				if (all->cmnd[all->i].fd_in > 0)
				{
					dup2(all->cmnd[all->i].fd_in, 0);
					close(all->cmnd[all->i].fd_in);
				} else
					dup2(all->fd_tmp, 0);
				close(all->fd[0]);
				if (all->cmnd[all->i].fd_out > 0)
					dup2(all->cmnd[all->i].fd_out, 1);
				else if (all->i != all->pip_count)
					dup2(all->fd[1], 1);
				close(all->fd[1]);
				path = get_data_path(all);
				envs_list_to_array(all);
				if (execve(path, all->cmnd[all->i].args, all->envp) == -1)
				{
					ft_putstr_fd(all->cmnd[all->i].args[0], 2);
					ft_putstr_fd(" : command not found\n", 2);
					exit(-10);
				}
				exit(10);
			}
		}
		close(all->fd[1]);
		all->fd_tmp = all->fd[0];
		all->i++;
	}
	int i = -1;
	while (++i < all->pip_count + 1)
		wait(NULL);
}