#include "minishell.h"

void cmd_exec1(t_all *all) //for multi pipes
{
	char *path;

		path = get_data_path(all);
		envs_list_to_array(all); //todo free
		if (execve(path, all->cmnd[all->i].args, all->envp) == -1)
		{
			ft_memdel(path);
			ft_memdel_double(all->envp);
			if (errno == 14)
				exec_error_print(all->cmnd[all->i].args[0], "command not found");
			else
				exec_error_print(all->cmnd[all->i].args[0], strerror(errno));
			exit (errno);
		}
		ft_memdel(path);
		ft_memdel_double(all->envp);
		exit(0);

}

void cmd_exec(t_all *all)// for no pipes
{
	char *path;

	pid_t	pid;

	pipe(all->fd);
	pid = fork();

	if (pid == -1)
	{
		exec_error_print("fork", strerror(errno));
		exit(errno);
	}
	else if (pid == 0)
	{
		if (all->cmnd[all->i].fd_in > STDIN_FILENO)
		{
			dup2(all->cmnd[all->i].fd_in, STDIN_FILENO);
			close(all->cmnd[all->i].fd_in);
		}
		close(all->fd[0]);

		if (all->cmnd[all->i].fd_out > STDOUT_FILENO)
		{
			dup2(all->cmnd[all->i].fd_out, STDOUT_FILENO);
			close(all->cmnd[all->i].fd_out);
		}
		close(all->fd[1]);//todo check dup2 fds!

		path = get_data_path(all);
		envs_list_to_array(all);

		if (execve(path, all->cmnd[all->i].args, all->envp) == -1)
		{
			ft_memdel(path);
			ft_memdel_double(all->envp);
			if (errno == 14)
				exec_error_print(all->cmnd[all->i].args[0], "command not found");
			else
				exec_error_print(all->cmnd[all->i].args[0], strerror(errno));
			exit (errno);
		}
		ft_memdel(path);
		ft_memdel_double(all->envp);
		exit(0);
	}
	close(all->fd[1]);
	close(all->fd[0]);
	int wstat;

	waitpid(pid, &wstat, 0);
	if (WIFEXITED(wstat))
	{
		int exit_code = WEXITSTATUS(wstat);
		if (exit_code != 0)
		{
			if (exit_code == 13)
				g_status_exit_code= 126;
			else if (exit_code == 14)
				g_status_exit_code = 127;
			else
				g_status_exit_code = exit_code;
		}
//		else
//			g_status_exit_code = 0;
	}
}



char *get_data_path(t_all *all)
{
	char **path;
	char *path_tmp;
	char *tmp;
	int i;
	struct stat	buff;

	buff.st_mode = 0;

	if (ft_strchr(all->cmnd[all->i].args[0], '/'))
		return (ft_strdup(all->cmnd[all->i].args[0]));

	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "PATH", 5) == 0)
			break;
	}
	if (i == all->env_counter)
		return(ft_strdup(all->cmnd[all->i].args[0]));

	path = ft_split(all->env_vars[i].value, ':');
	if (!path)
		return(NULL);

	i = -1;
	path_tmp = NULL;
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		path_tmp = ft_strjoin(tmp, all->cmnd[all->i].args[0]);
		ft_memdel(tmp);
		stat(path_tmp, &buff);
		if (buff.st_mode != 0)
			break;
		ft_memdel(path_tmp);
	}
	ft_memdel_double(path);
	if (buff.st_mode)
		return(path_tmp);
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