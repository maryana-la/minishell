/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 14:05:52 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/13 15:13:00 by jjacquel         ###   ########.fr       */
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
	(znak == 1 && chislo > 9223372036854775807 ? chislo = -1 : 0);
	(znak == -1 && chislo > 9223372036854775807 ? chislo = 0 : 0);
	return (chislo * znak);
}
