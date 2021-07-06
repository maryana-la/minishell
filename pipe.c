//
// Created by Ragwyle Chelsea on 7/6/21.
//

#include "minishell.h"


void	call_child(t_all *all, int fd_tmp)
{
	if (all->cmnd[all->i].fd_in > 0)
	{
		dup2(all->cmnd[all->i].fd_in, 0);
		close(all->cmnd[all->i].fd_in);
	}
	else
		dup2(fd_tmp, 0);
	close(all->fd[0]);


	if (all->cmnd[all->i].fd_out > 0)
		dup2(all->cmnd[all->i].fd_out, 1);
	else if (all->i != all->pip_count)
		dup2(all->fd[1], 1);
	close(all->fd[1]);

	start_commands(all);


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
	exit(0);

}


void 	launch_commands(t_all *all)
{
	int fd_tmp;
	pid_t	pid;

	all->i = 0;
	while(all->i < all->pip_count + 1)
	{
		pipe(all->fd);
		pid = fork();
		if (pid == -1)
			exit (-30);
//			ft_error_exit("fork", &pip, FORK_ERR);
		else if (pid == 0)
			call_child(all, fd_tmp);
		close(all->fd[1]);
		fd_tmp = all->fd[0];
		all->i++;
	}
	wait(NULL);
}