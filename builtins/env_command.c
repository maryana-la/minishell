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

void	print_env_list_1(t_env *for_print, int i)
{
	write (1, "declare -x ", 11);
	write(1, for_print[i].key, ft_strlen(for_print[i].key));
	if (ft_strcmp(for_print[i].value, "nullvalue"))
	{
		write(1, "=", 1);
		write(1, "\"", 1);
		write(1, for_print[i].value, ft_strlen(for_print[i].value));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void	print_env_list(t_env *for_print, int declare, int num_of_vars)
{
	int	i;

	i = 0;
	while (i < num_of_vars && for_print[i].key && for_print[i].key[0] != '\0')
	{
		if (declare)
			print_env_list_1(for_print, i);
		else
		{
			if (ft_strcmp(for_print[i].value, "nullvalue"))
			{
				write(1, for_print[i].key, ft_strlen(for_print[i].key));
				write(1, "=", 1);
				write(1, for_print[i].value, ft_strlen(for_print[i].value));
				write(1, "\n", 1);
			}
		}
		i++;
	}
}
