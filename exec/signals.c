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

void	get_exit_status(int wstat)
{
	int	exit_code;

	if (WIFEXITED(wstat))
	{
		exit_code = WEXITSTATUS(wstat);
		if (exit_code != 0)
		{
			if (exit_code == 13)
				g_status_exit_code = 126;
			else if (exit_code == 14)
				g_status_exit_code = 127;
			else
				g_status_exit_code = exit_code;
		}
		else
			g_status_exit_code = 0;
	}
}

void	sig_handler(int sig_id)
{
	int		code;

	wait(&code);
	if (code == sig_id)
	{
		if (sig_id == SIGINT)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else if (sig_id == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", 2);
		}
		g_status_exit_code = 128 + sig_id;
	}
	else if (sig_id == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_status_exit_code = 1;
	}
}
