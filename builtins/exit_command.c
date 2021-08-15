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

void	print_and_exit(t_all *all, int err)
{
	if (all->num_of_pipes == 0)
		ft_putendl_fd("exit", 2);
	exit(err);
}

void	exit_command_1(t_all *all)
{
	int	i;

	i = -1;
	while (all->cmnd[all->i].args[1][++i])
	{
		if (all->cmnd[all->i].args[1][i] == '-' && i == 0)
			continue ;
		if (!ft_isdigit(all->cmnd[all->i].args[1][i]))
		{
			ft_putendl_fd("exit", 2);
			exec_error_print(all->cmnd[all->i].args[1], \
			"numeric argument required");
			ft_free_env(all->env_vars);
			exit (255);
		}
	}
}

void	exit_command_2(t_all *all)
{
	int	error_code;

	error_code = ft_atoi(all->cmnd[all->i].args[1]);
	if (error_code < 0)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, 256 + error_code % 256);
	}
	else if (error_code < 256)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, error_code);
	}
	else
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, error_code % 256);
	}
}

void	exit_command(t_all *all)
{
	int	i;

	i = -1;
	while (all->cmnd[all->i].args[++i])
		;
	if (i == 1)
	{
		ft_free_env(all->env_vars);
		ft_free_commands(all);
		print_and_exit(all, 0);
	}
	if (i > 2)
	{
		exec_error_print("exit", "too many arguments");
		g_status_exit_code = 1;
		return ;
	}
	exit_command_1(all);
	exit_command_2(all);
}
