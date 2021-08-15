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

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_all	all;
	char	*str;

	g_status_exit_code = 0;
	(void)argc;
	(void)argv;
	all.cmnd = NULL;
	init_all(&all);
	env_init(&all, env);
	all.fd_std[0] = dup(0);
	all.fd_std[1] = dup(1);
	if ((signal(SIGINT, sig_handler) == SIG_ERR) || \
	(signal(SIGQUIT, sig_handler) == SIG_ERR))
	printf("Signal init error\n");
	while (1)
	{
		if (take_input(&all, &str))
			continue ;
		if (ft_preparser(str, &all) == 0)
			parse_and_exec(str, &all);
		else
			ft_memdel(str);
		init_all(&all);
	}
	return (0);
}

void	ft_free_commands(t_all *all)
{
	int	i;

	i = 0;
	if (all->cmnd)
	{
		while (all->cmnd[i].args)
		{
			ft_memdel_double(all->cmnd[i].args);
			i++;
		}
		free(all->cmnd);
		all->cmnd = NULL;
	}
}

void	init_all(t_all *all)
{
	ft_free_commands(all);
	all->num_of_pipes = 0;
}

int	not_empty_line(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (i < ft_strlen(str))
		return (1);
	return (0);
}

int	take_input(t_all *all, char **str)
{
	char	*buf;

	rl_catch_signals = 0;
	buf = readline(GREEN "minishell> " RESET);
	if (!buf)
		print_and_exit(all, 0);
	if (ft_strlen(buf) != 0 && not_empty_line(buf))
	{
		add_history(buf);
		*str = ft_strdup(buf);
		buf = ft_memdel(buf);
		return (0);
	}
	else
	{
		buf = ft_memdel(buf);
		return (1);
	}
}
