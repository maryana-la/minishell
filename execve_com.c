#include "minishell.h"

void cmd_exec1(t_all *all) //for multi pipes
{
	char *path;

		path = get_data_path(all);
		envs_list_to_array(all);
		if (execve(path, all->cmnd[all->i].args, all->envp) == -1)
		{
			if (errno == 14)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(all->cmnd[all->i].args[0], 2);
				ft_putstr_fd(" : command not found\n", 2);
			}
			else
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(all->cmnd[all->i].args[0], 2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd("\n", 2);
			}
			exit (errno);
		}
		exit(0);

}

void cmd_exec(t_all *all)// for no pipes
{
	char *path;

	pid_t	pid;

	pipe(all->fd); //new
	pid = fork();

	if (pid == -1)
		exit(-11);
	else if (pid == 0)
	{
		if (all->cmnd[all->i].fd_in > STDIN_FILENO)
		{
			dup2(all->cmnd[all->i].fd_in, STDIN_FILENO);
			close(all->cmnd[all->i].fd_in);
		}
		close(all->fd[0]);

		if (all->cmnd[all->i].fd_out > STDOUT_FILENO)
			dup2(all->cmnd[all->i].fd_out, STDOUT_FILENO);
		close(all->fd[1]);

		path = get_data_path(all);
		envs_list_to_array(all);

		if (execve(path, all->cmnd[all->i].args, all->envp) == -1)
		{
			if (errno == 14)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(all->cmnd[all->i].args[0], 2);
				ft_putstr_fd(" : command not found\n", 2);
			}
			else
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(all->cmnd[all->i].args[0], 2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd("\n", 2);
			}
			exit (errno);
		}
		exit(0);
	}
	close(all->fd[1]);
	close(all->fd[0]);
	int wstat;

	waitpid(pid, &wstat, 0);
//	if (WIFSIGNALED(wstat))
//	{
//		int temp;
//		temp = WTERMSIG(wstat);
//		if (temp == SIGINT)
//			all->last_exit = 130;
//		else if (temp == SIGQUIT)
//		{
//			all->last_exit = 131;
//			printf("Quit: 3\n");
//		}
//	}

	if (WIFEXITED(wstat))
	{
		int exit_code = WEXITSTATUS(wstat);
		if (exit_code != 0)
		{
			if (exit_code == 13)
				all->last_exit = 126;
			else if (exit_code == 14)
				all->last_exit = 127;
			else
				all->last_exit = exit_code;
		}
//		else
//			all->last_exit = 0;
	}
}



char *get_data_path(t_all *all)
{
	char **path;
	char *path_tmp;
	char *tmp;
	int i;

	if (ft_strchr(all->cmnd[all->i].args[0], '/'))
		return (ft_strdup(all->cmnd[all->i].args[0]));

	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "PATH", 5) == 0)
			break;
	}
	path = ft_split(all->env_vars[i].value, ':');

	if (!path)
	{
//		ft_free_array(*arg_data); todo Maryana free all
		exit(-1);
	}
	i = -1;
	path_tmp = NULL;
	while (path[++i])
	{
		path_tmp = ft_strjoin(path[i], "/");
		tmp = path_tmp;
		path_tmp = ft_strjoin(path_tmp, all->cmnd[all->i].args[0]);
		ft_memdel(tmp);
		if (access(path_tmp, F_OK | X_OK) == 0) //todo Maryana replace access with read
		{
//			ft_free_array(path); //todo free
			return (path_tmp);
		}
		ft_memdel(path_tmp);
	}
	return (NULL);
}

void envs_list_to_array(t_all *all)
{
	int i;
	char *tmp;

	all->envp = malloc(sizeof(char *) * (all->env_counter + 1));
	i = -1;
	while(all->env_vars[++i].key)
	{
		if (all->env_vars[i].value != NULL)
		{
			all->envp[i] = ft_strjoin(all->env_vars[i].key, "=");
			if (all->env_vars[i].value[0] == '\0')
				continue;
			else
			{
				tmp = all->envp[i];
				all->envp[i] = ft_strjoin(all->envp[i], all->env_vars[i].value);
				ft_memdel(tmp);
			}
		}
	}
	all->envp[i] = NULL;
}