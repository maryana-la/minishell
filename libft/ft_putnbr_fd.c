/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 20:27:19 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/10 15:44:48 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	t;

	t = 0;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		t = n * -1;
	}
	else
		t = n;
	if (t > 9)
		ft_putnbr_fd(t / 10, fd);
	ft_putchar_fd(t % 10 + 48, fd);
}
