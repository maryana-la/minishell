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

int	ft_preparser(char *str, t_all *all)
{
	int	i;
	int	len;

	all->num_of_pipes = 0;
	len = (int)ft_strlen(str) - 1;
	i = 0;
	skip_spaces(str, &i);
	while (str[len] == ' ' || str[len] == '\t')
		len--;
	if (str[i] == '|' || str[len] == '|')
	{
		if (str[i] == str[i + 1] || str[len] == str[len - 1])
			return (preparser_error("||", 2));
		else
			return (preparser_error("|", 1));
	}
	if (str[len] == '<' || str[len] == '>')
		return (preparser_error("newline", 7));
	i--;
	return (ft_preparser_1(str, all, i));
}

int	ft_preparser_1(char *str, t_all *all, int i)
{
	while (str[++i])
	{
		if (str[i] == '\\' || str[i] == ';')
			return (preparser_error(&str[i], 1));
		else if (str[i] == '\'')
		{
			if (check_inside_s_quote(str, &i) == 1)
				return (preparser_error("\'", 1));
		}
		else if (str[i] == '\"')
		{
			if (check_inside_d_quote(str, &i) == 1)
				return (preparser_error("\"", 1));
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (str[i] == '|')
				all->num_of_pipes++;
			if (check_tokens(str, &i, str[i]) == 1)
				return (preparser_error(&str[i], 1));
		}
	}
	return (0);
}

int	preparser_error(char *token, int len)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, token, len);
	write (2, "'\n", 2);
	return (g_status_exit_code = 258);
}
