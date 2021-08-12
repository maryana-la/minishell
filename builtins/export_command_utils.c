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

void	add_new_variable_3(t_all *all, char *temp_key, char *temp_value)
{
	int		i;
	t_env	*tmp;

	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup(temp_key);
	if ((!temp_value) && !all->ravno)
		tmp[i].value = ft_strdup("nullvalue");
	else
		tmp[i].value = ft_strdup(temp_value);
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
	ft_memdel(temp_key);
	ft_memdel(temp_value);
	g_status_exit_code = 0;
}

int	add_new_variable_2(t_all *all, char *temp_key, char *temp_value)
{
	char	*to_free;
	int		i;

	i = -1;
	while (++i < all->env_counter && ft_strcmp(all->env_vars[i].key, temp_key))
		;
	if (i != all->env_counter && (all->ravno == 0))
	{
		g_status_exit_code = 0;
		ft_memdel(temp_key);
		return (-1);
	}
	if (i != all->env_counter)
	{
		to_free = all->env_vars[i].value;
		all->env_vars[i].value = ft_strdup(temp_value);
		ft_memdel(temp_key);
		ft_memdel(temp_value);
		ft_memdel(to_free);
		g_status_exit_code = 0;
		return (-1);
	}
	return (1);
}

int	add_new_variable_1(t_all *all)
{
	if (!ft_isalpha(all->cmnd[all->i].args[all->arg_pos][0]) && \
	(all->cmnd[all->i].args[all->arg_pos][0] != '_'))
	{
		error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
		g_status_exit_code = 1;
		return (-1);
	}
	return (1);
}

int	add_new_variable_4(t_all *all, int *i)
{
	*i = -1;
	while (all->cmnd[all->i].args[all->arg_pos][++(*i)])
	{
		if (all->cmnd[all->i].args[all->arg_pos][*i] == '=')
		{
			all->ravno = 1;
			break ;
		}
		else if (!ft_isalnum(all->cmnd[all->i].args[all->arg_pos][*i]) \
		&& (all->cmnd[all->i].args[all->arg_pos][0] != '_'))
		{
			error_handler(all->cmnd[all->i].args[all->arg_pos], 1);
			g_status_exit_code = 1;
			return (-1);
		}
	}
	return (1);
}

void	add_new_variable(t_all *all)
{
	int		i;
	char	*temp_key;
	char	*temp_value;

	if (add_new_variable_1(all) == -1)
		return ;
	all->ravno = 0;
	if (add_new_variable_4(all, &i) == -1)
		return ;
	temp_key = ft_substr(all->cmnd[all->i].args[all->arg_pos], 0, i);
	temp_value = NULL;
	if (all->ravno != 0)
		temp_value = ft_substr(all->cmnd[all->i].args[all->arg_pos], i + 1, \
		ft_strlen(all->cmnd[all->i].args[all->arg_pos]) - i + 1);
	if (add_new_variable_2(all, temp_key, temp_value) == -1)
		return ;
	add_new_variable_3(all, temp_key, temp_value);
}
