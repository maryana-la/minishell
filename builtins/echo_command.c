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

void	echo_command_1(t_all *all, int i)
{
	while (all->cmnd[all->i].args[++i])
	{
		if (all->cmnd[all->i].args[i + 1])
		{
			ft_putstr_fd(all->cmnd[all->i].args[i], 1);
			ft_putstr_fd(" ", 1);
		}
		else
			ft_putstr_fd(all->cmnd[all->i].args[i], 1);
	}
}

void	echo_command(t_all *all)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	flag = 0;
	while (all->cmnd[all->i].args[++i])
	{
		if (!(ft_strncmp(all->cmnd[all->i].args[i], "-n", 2)))
		{
			j = 0;
			while (all->cmnd[all->i].args[i][++j] == 'n')
				;
			if (j == (int)ft_strlen(all->cmnd[all->i].args[i]))
				flag = 1;
			else
				break ;
		}
		else
			break ;
	}
	i--;
	echo_command_1(all, i);
	if (!flag)
		ft_putstr_fd("\n", 1);
}
