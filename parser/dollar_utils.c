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

char	*if_question_mark(char *str, int pos_of_dollar, int *i, char *var)
{
	char	*begin_of_str;
	char	*end_of_line;
	char	*tmp;
	char	*value;
	char	*str1;

	value = ft_itoa(g_status_exit_code);
	begin_of_str = ft_substr(str, 0, pos_of_dollar);
	end_of_line = ft_substr(str, (pos_of_dollar + 2), \
	(ft_strlen(str) - pos_of_dollar - 1));
	tmp = ft_strjoin(begin_of_str, value);
	str1 = ft_strjoin(tmp, end_of_line);
	*i = pos_of_dollar + (int)ft_strlen(value) - 1;
	ft_memdel(tmp);
	ft_memdel(begin_of_str);
	ft_memdel(value);
	ft_memdel(end_of_line);
	ft_memdel(var);
	return (str1);
}

char	*if_numeric(char *str, int pos_of_dollar, int *i, char *var)
{
	char	*begin_of_str;
	char	*end_of_line;
	char	*str1;

	if (ft_strlen(var) == 1)
		end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
	else
	{
		end_of_line = ft_substr(str, pos_of_dollar + 2, \
		(ft_strlen(str) - pos_of_dollar - 1));
	}
	begin_of_str = ft_substr(str, 0, pos_of_dollar);
	str1 = ft_strjoin(begin_of_str, end_of_line);
	ft_memdel(begin_of_str);
	ft_memdel(end_of_line);
	ft_memdel(var);
	return (str1);
}
