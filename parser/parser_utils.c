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

int	get_arg_len(char *str, int i)
{
	int	len;

	len = 0;
	while (str[i] && !check_set(str[i], " \t|;<>"))
	{
		if (str[i] == '\'')
			count_inside_quotes(str, &i, &len, 1);
		else if (str[i] == '\"')
			count_inside_quotes(str, &i, &len, 2);
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

void	count_inside_quotes(const char *str, int *i, int *len, int type)
{
	char	quote;

	if (type == 1)
		quote = '\'';
	else
		quote = '\"';
	(*i)++;
	while (str[*i] != quote)
	{
		(*len)++;
		(*i)++;
	}
	(*i)++;
}

int	check_set(char c, const char *set)
{
	int	i;

	i = -1;
	while (set[++i] != '\0')
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

void	skip_spaces(const char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
}

void	find_next_cmnd(const char *str, int *i)
{
	while (str[*i] && str[*i] != '|')
		(*i)++;
}
