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

void	find_end_of_word(char *str, int *x)
{
	while (str[*x] && !(check_set(str[*x], " \t|;<>")))
	{
		if (str[*x] == '\'')
		{
			(*x)++;
			while (str[*x] != '\'')
				(*x)++;
			(*x)++;
		}
		else if (str[*x] == '\"')
		{
			(*x)++;
			while (str[*x] != '\"')
				(*x)++;
			(*x)++;
		}
		else
			(*x)++;
	}
}

char	*cut_name_from_str(char *str_new, int *i, int x, int len)
{
	char	*tmp;
	int		j;

	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
	{
		*i = x;
		ft_memdel(str_new);
		return (NULL);
	}
	j = 0;
	while (str_new[*i] && !check_set(str_new[*i], " \t|;<>"))
	{
		get_argument(str_new, i, tmp, &j);
		(*i)++;
		j++;
	}
	tmp[j] = '\0';
	ft_memdel(str_new);
	*i = x;
	return (tmp);
}

char	*get_file_name(char *str, int *i, int type, t_all *all)
{
	int		len;
	int		x;
	char	*str_new;

	skip_spaces(str, i);
	x = *i;
	find_end_of_word(str, &x);
	if (type != 2)
		str_new = replace_env_with_value(str, *i, all);
	else
		str_new = ft_strdup(str);
	len = get_arg_len(str_new, *i);
	if (!len)
	{
		*i = x;
		ft_memdel(str_new);
		return (NULL);
	}
	return (cut_name_from_str(str_new, i, x, len));
}
