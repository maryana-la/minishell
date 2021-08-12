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

void	export_command(t_all *all)
{
	all->arg_pos = 0;
	if (!all->cmnd[all->i].args[1])
	{
		sort_envs(all);
		print_env_list(all->env_sorted, 1, all->env_counter);
		ft_memdel(all->env_sorted);
		return ;
	}
	while (all->cmnd[all->i].args[++all->arg_pos])
	{
		add_new_variable(all);
	}
}

void	sort_envs_1(t_all *all, int i)
{
	t_env	tmp;

	tmp = all->env_sorted[i];
	all->env_sorted[i] = all->env_sorted[i + 1];
	all->env_sorted[i + 1] = tmp;
}

void	sort_envs(t_all *all)
{
	int		i;
	int		z;
	int		j;

	i = -1;
	z = -1;
	all->env_sorted = malloc(sizeof (t_env) * (all->env_counter + 1));
	while (all->env_vars[++i].key)
		all->env_sorted[i] = all->env_vars[i];
	while (++z < all->env_counter - 2)
	{
		i = all->env_counter - 1;
		while (--i > z - 1)
		{
			j = 0;
			while (all->env_sorted[i].key[j] == all->env_sorted[i + 1].key[j])
				j++;
			if (all->env_sorted[i].key[j] > all->env_sorted[i + 1].key[j])
				sort_envs_1(all, i);
		}
	}
}
