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

void	start_commands(t_all *all)
{
	if (is_builtin(all->cmnd[all->i].args[0]))
		buitin_commands(all);
	else
	{
		if (all->pip_count == 0)
			cmd_exec(all);
		else
			cmd_exec_child(all);
	}
}

void	buitin_commands(t_all *all)
{
	if (all->pip_count == 0)
		dup2_if_redirect(all);
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
}

int	is_builtin(char *command)
{
	if (!ft_strncmp(command, "pwd", 4) || !ft_strncmp(command, "env", 4) || \
	!ft_strncmp(command, "export", 7) || !ft_strncmp(command, "unset", 6) \
	|| !ft_strncmp(command, "cd", 3) || !ft_strncmp(command, "echo", 5) \
	|| !ft_strncmp(command, "exit", 5))
		return (1);
	return (0);
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
