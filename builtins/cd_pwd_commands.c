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

void	cd_command_1(t_all *all)
{
	int		i;
	char	*tmp;

	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "OLDPWD"))
	{
	}
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(all->tmp_cwd);
		ft_memdel (tmp);
	}
	ft_memdel(all->tmp_cwd);
	getcwd(all->cwd, sizeof(all->cwd));
	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, "PWD"))
		;
	if (i != all->env_counter)
	{
		tmp = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(all->cwd);
		ft_memdel (tmp);
	}
}

void	cd_command(t_all *all)
{
	int	i;

	i = -1;
	all->tmp_cwd = getcwd(NULL, 0);
	if (!all->cmnd[all->i].args[1])
	{
		while (++i < all->env_counter && \
		ft_strcmp(all->env_vars[i].key, "HOME"))
			;
		if (i != all->env_counter)
		{
			chdir(all->env_vars[i].value);
		}
	}
	else if (chdir(all->cmnd[all->i].args[1]) == -1)
	{
		printf("minishell: cd: %s: %s\n", \
			all->cmnd[all->i].args[1], strerror(errno));
		ft_memdel(all->tmp_cwd);
		g_status_exit_code = 1;
		return ;
	}
	cd_command_1(all);
}

void	pwd_command(t_all *all)
{
	getcwd(all->cwd, sizeof(all->cwd));
	write(1, all->cwd, ft_strlen(all->cwd));
	write(1, "\n", 1);
}
