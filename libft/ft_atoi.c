/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 14:05:52 by jjacquel          #+#    #+#             */
/*   Updated: 2021/06/29 18:35:54 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	unsigned long int		chislo;
	int						znak;

	chislo = 0;
	znak = 1;
	while ((*str == 32) || ((*str >= 9) && (*str <= 13)))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		znak = znak * -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		chislo = chislo * 10 + *str - '0';
		str++;
	}
	return (chislo * znak);
}
