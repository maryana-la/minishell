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

void	parse_and_exec(char *str, t_all *all)
{
	int	i;

	all->cmnd = malloc(sizeof(t_cmnd) * (all->num_of_pipes + 2));
	i = -1;
	while (++i < (all->num_of_pipes + 2))
	{
		all->cmnd[i].args = NULL;
		all->cmnd[i].fd_in = STDIN_FILENO;
		all->cmnd[i].fd_out = STDOUT_FILENO;
	}
	all->pip_count = 0;
	ft_parse(all, str, 0);
	if (all->cmnd->args)
		launch_commands(all);
}

void	ft_parse(t_all *all, char *str, int i)
{
	char	*old_str;

	while (str[i])
	{
		skip_spaces(str, &i);
		if (str[i] == '>' || str[i] == '<')
		{
			if (ft_handle_redirect(str, &i, all))
				find_next_cmnd(str, &i);
		}
		else if (str[i] == '|')
		{
			all->pip_count++;
			i++;
		}
		else
		{
			old_str = str;
			str = replace_env_with_value(str, i, all);
			ft_memdel(old_str);
			get_all_args(all, str, &i);
		}
	}
	ft_memdel(str);
}

void	get_all_args(t_all *all, char *str, int *i)
{
	char	*tmp;
	int		len;
	int		j;

	len = get_arg_len(str, *i);
	tmp = ft_calloc((len + 2), sizeof (char));
	if (!tmp)
		return ;
	j = 0;
	while (str[*i] && !check_set(str[*i], " \t|;<>"))
	{
		get_argument(str, i, tmp, &j);
		(*i)++;
		j++;
	}
	if (len > 0)
	{
		tmp[j] = '\0';
		ft_put_str_to_struct(tmp, all);
	}
	ft_memdel(tmp);
}

void	get_argument(const char *str, int *i, char *tmp, int *j)
{
	if (str[*i] == '\'')
	{
		while (str[++(*i)] && str[*i] != '\'')
		{
			tmp[*j] = str[*i];
			(*j)++;
		}
		(*j)--;
	}
	else if (str[*i] == '\"')
	{
		while (str[++(*i)] && str[*i] != '\"')
		{
			tmp[*j] = str[*i];
			(*j)++;
		}
		(*j)--;
	}
	else
		tmp[*j] = str[*i];
}

void	ft_put_str_to_struct(char *arg, t_all *all)
{
	int		i;
	char	**tmp;

	if (!all->cmnd[all->pip_count].args)
	{
		all->cmnd[all->pip_count].args = malloc(sizeof(char *) * 2);
		all->cmnd[all->pip_count].args[0] = ft_strdup(arg);
		all->cmnd[all->pip_count].args[1] = NULL;
	}
	else
	{
		i = 0;
		while (all->cmnd[all->pip_count].args[i] != NULL)
			i++;
		tmp = malloc(sizeof(char *) * (i + 2));
		i = -1;
		while (all->cmnd[all->pip_count].args[++i] != 0)
			tmp[i] = ft_strdup(all->cmnd[all->pip_count].args[i]);
		tmp[i] = ft_strdup(arg);
		tmp[i + 1] = NULL;
		ft_memdel_double(all->cmnd[all->pip_count].args);
		all->cmnd[all->pip_count].args = tmp;
	}
}
