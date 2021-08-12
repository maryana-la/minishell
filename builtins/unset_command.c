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

int	unset_command_1(t_all *all, int j)
{
	if (!ft_isalpha(all->cmnd[all->i].args[j][0]) && \
	(all->cmnd[all->i].args[j][0] != '_'))
	{
		error_handler(all->cmnd[all->i].args[j], 2);
		all->arg_pos = j;
		g_status_exit_code = 1;
		return (-1);
	}
	return (1);
}

void	unset_command_2(t_all *all, int i)
{
	ft_memdel(all->env_vars[i].key);
	ft_memdel(all->env_vars[i].value);
	while (i < all->env_counter)
	{
		all->env_vars[i] = all->env_vars[i + 1];
		i++;
	}
}

void	unset_command(t_all *all)
{
	int	i;
	int	j;

	j = 0;
	while (all->cmnd[all->i].args[++j])
	{
		i = -1;
		if (unset_command_1(all, j) == -1)
			return ;
		while (++i < all->env_counter && \
		ft_strcmp(all->env_vars[i].key, all->cmnd[all->i].args[j]))
			;
		if (i != all->env_counter)
		{
			unset_command_2(all, i);
			--all->env_counter;
			continue ;
		}
		else
			continue ;
	}
}
