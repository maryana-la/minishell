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

void	increment_shlvl(char *line, int i, t_all *all, int *flag)
{
	int		shlvl_tmp;
	char	*tmp_str;

	all->env_vars[i].key = ft_strdup("SHLVL");
	tmp_str = ft_substr(line, 6, (ft_strlen(line) - 6));
	shlvl_tmp = ft_atoi(tmp_str) + 1;
	if (shlvl_tmp < 0)
		shlvl_tmp = 0;
	else if (shlvl_tmp > 1000)
	{
		printf("minishell: warning: shell level (%d) too high, resetting to 1", \
		shlvl_tmp);
		shlvl_tmp = 1;
	}
	all->env_vars[i].value = ft_itoa(shlvl_tmp);
	ft_memdel(tmp_str);
	*flag = 1;
}

void	set_shlvl(t_all *all)
{
	int		i;
	t_env	*tmp;

	i = -1;
	tmp = malloc(sizeof (t_env) * (all->env_counter + 2));
	while (all->env_vars[++i].key)
		tmp[i] = all->env_vars[i];
	tmp[i].key = ft_strdup("SHLVL");
	tmp[i].value = ft_strdup("1");
	tmp[i + 1].key = NULL;
	tmp[i + 1].value = NULL;
	ft_memdel(all->env_vars);
	all->env_vars = tmp;
	all->env_counter++;
}
