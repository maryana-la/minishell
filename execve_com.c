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
				perror(all->cmnd[all->i].args[0]);
			printf("%d\n", errno);
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
			dup2(all->cmnd[all->i].fd_in, 0);
			close(all->cmnd[all->i].fd_in);
		}
		close(all->fd[0]);

		if (all->cmnd[all->i].fd_out > 1)
			dup2(all->cmnd[all->i].fd_out, 1);
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
				perror(all->cmnd[all->i].args[0]);
			printf("%d\n", errno);
			exit (errno); // todo 14-bad address = 127 command not found, 13 - permission denied 126
		}
		exit(0);
	}
	close(all->fd[1]);
	close(all->fd[0]);
	int wstat;
	wait(&wstat);
	printf("wstat=%d\n", wstat);
	if (WIFEXITED(wstat))
	{
		int exit_code = WEXITSTATUS(wstat);
		if (exit_code != 0) // todo do nothing
		{
			if (exit_code == 13)
				all->last_exit = 126;
			else if (exit_code == 14)
				all->last_exit = 127;
			else
				all->last_exit = exit_code;
			printf("Error %d\n", all->last_exit);
		}
		else
		{
			printf("Success\n");
			all->last_exit = 0;
		}
	}
	else
	{
		if (wstat == SIGINT)
			all->last_exit = 130;
		else if (wstat == SIGQUIT)
			all->last_exit = 130;
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
		free(tmp);
		if (access(path_tmp, F_OK | X_OK) == 0) //todo Maryana replace access with read
		{
//			ft_free_array(path); //todo free
			return (path_tmp);
		}
		free(path_tmp);
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
				free(tmp);
			}
		}
	}
}