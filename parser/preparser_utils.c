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

int	check_inside_s_quote(char *str, int *i)
{
	*i = *i + 1;
	while (str[*i] && str[*i] != '\'')
		*i = *i + 1;
	if (str[*i] == '\'')
		return (0);
	return (1);
}

int	check_inside_d_quote(char *str, int *i)
{
	*i = *i + 1;
	while (str[*i])
	{
		if (str[*i] == '\"' && str[*i - 1] != '\\')
			break ;
		*i = *i + 1;
	}
	if (str[*i] == '\"')
		return (0);
	return (1);
}

int	check_tokens(char *str, int *i, char token)
{
	if (check_tokens_1(str, i, token) == 1)
		return (1);
	if (token == '|')
	{
		if (str[*i] == ';' || str[*i] == token || (str[*i] == '<' && str[*i \
		+ 1] == '<' ) || str[*i] == '<' || (str[*i] == '>' \
		&& str[*i + 1] == '>') || str[*i] == '>')
			return (1);
	}
	if (token == '>')
	{
		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') \
		|| str[*i] == '|' || str[*i] == ';')
			return (1);
	}
	if (token == '<')
	{
		if (str[*i] == '|' || str[*i] == ';' || (str[*i] == '>'))
			return (1);
	}
	*i = *i - 1;
	return (0);
}

int	check_tokens_1(char *str, int *i, char token)
{
	int	first;
	int	diff;

	first = *i;
	*i = *i + 1;
	while (str[*i] == ' ' || str[*i] == '\t')
		*i = *i + 1;
	diff = *i - first;
	if (diff > 1 && str[*i] == token)
		return (1);
	return (0);
}
