#include "minishell.h"

void cmd_exec(t_all *all)
{
	char *path;
	int fd[2];

	pid_t	pid;

		pipe(fd);
		pid = fork();
		if (pid == -1)
//			ft_error_exit("fork", &pip, FORK_ERR);
			exit(-11);
		else if (pid == 0)
		{
			close(fd[0]);
			if (all->cmnd.fd_in != -1)
				dup2(all->cmnd.fd_in, 0);
			if (all->cmnd.fd_out != -1)
				dup2(all->cmnd.fd_out, 1);
			path = get_data_path(all);
			envs_list_to_array(all);
			if (execve(path, all->cmnd.args, all->envp) == -1)
			{
	//		ft_free_line(pip->path);
	//		ft_free_array(pip->arg_data);
//				perror(all->cmnd.args[0]);
			ft_putstr_fd(all->cmnd.args[0], 2);
			ft_putstr_fd(" : command not found\n", 2);
				exit(-10);
			}
		}
		wait(NULL);
		close(fd[1]);
}

char *get_data_path(t_all *all)
{
	char **path;
	char *path_tmp;
	char *tmp;
	char *get_pwd;
	char *exec;
	int i;

	if (ft_strncmp(all->cmnd.args[0], "./", 2) == 0)
	{
		exec = ft_substr(all->cmnd.args[0], 2, ft_strlen(all->cmnd.args[0]) - 2);
		if (!(get_pwd = getcwd(NULL, -1)))
		{
			perror("getcwd");
		}
		tmp = ft_strjoin(get_pwd, "/");
		free(get_pwd);
		get_pwd = ft_strjoin(tmp, exec);
		free(tmp);
		if (access(get_pwd, X_OK) != 0) // todo MAryana replace access with read
		{
			free(get_pwd);
			perror (all->cmnd.args[0]);
			exit (1);
		}
		return (get_pwd);
	}

	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "PATH", 5) == 0)
			break;
	}
	path = ft_split(all->env_vars[i].value, ':');

	if (!path)
	{
//		ft_free_array(*arg_data); todo MAryana free all
		exit(-1);
	}
	i = -1;
	path_tmp = NULL;
	while (path[++i])
	{
		path_tmp = ft_strjoin(path[i], "/");
		tmp = path_tmp;
		path_tmp = ft_strjoin(path_tmp, all->cmnd.args[0]);
		free(tmp);
		if (access(path_tmp, F_OK | X_OK) == 0) //todo Maryana replace access with read
		{
//			ft_free_array(path); to free
			return (path_tmp);
		}
		free(path_tmp);
	}
	return (0);
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