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

/*
 * 	execution for multi pipes
 */

void	cmd_exec_child(t_all *all)
{
	char	*path;

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

/*
 *  execution for no pipes
 */

void	cmd_exec(t_all *all)
{
	pid_t	pid;
	int		wstat;

	pipe(all->fd);
	pid = fork();
	if (pid == -1)
	{
		exec_error_print("fork", strerror(errno));
		exit(errno);
	}
	else if (pid == 0)
	{
		dup2_if_redirect(all);
		close(all->fd[0]);
		close(all->fd[1]);
		cmd_exec_child(all);
	}
	close(all->fd[1]);
	close(all->fd[0]);
	waitpid(pid, &wstat, 0);
	get_exit_status(wstat);
}

char	*sort_out_path(t_all *all, int i)
{
	char		**path;
	char		*tmp;
	char		*path_tmp;
	struct stat	buff;

	buff.st_mode = 0;
	path = ft_split(all->env_vars[i].value, ':');
	if (!path)
		return (NULL);
	i = -1;
	path_tmp = NULL;
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		path_tmp = ft_strjoin(tmp, all->cmnd[all->i].args[0]);
		ft_memdel(tmp);
		stat(path_tmp, &buff);
		if (buff.st_mode != 0)
			break ;
		ft_memdel(path_tmp);
	}
	ft_memdel_double(path);
	if (buff.st_mode)
		return (path_tmp);
	return (NULL);
}

char	*get_data_path(t_all *all)
{
	int	i;

	if (ft_strchr(all->cmnd[all->i].args[0], '/'))
		return (ft_strdup(all->cmnd[all->i].args[0]));
	i = -1;
	while (all->env_vars[++i].key)
	{
		if (ft_strncmp(all->env_vars[i].key, "PATH", 5) == 0)
			break ;
	}
	if (i == all->env_counter)
		return (ft_strdup(all->cmnd[all->i].args[0]));
	return (sort_out_path(all, i));
}

void	envs_list_to_array(t_all *all)
{
	int		i;
	char	*tmp;

	all->envp = malloc(sizeof(char *) * (all->env_counter + 1));
	i = -1;
	while (all->env_vars[++i].key)
	{
		if (all->env_vars[i].value != NULL)
		{
			all->envp[i] = ft_strjoin(all->env_vars[i].key, "=");
			if (all->env_vars[i].value[0] == '\0')
				continue ;
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
