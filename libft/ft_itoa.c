/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 16:07:40 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/22 19:04:32 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_lenght(unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (n > 9)
	{
		n = n / 10;
		i++;
	}
	return (i + 1);
}

char	*ft_itoa(int n)
{
	char			*chr;
	unsigned int	dlina;
	unsigned int	num;
	unsigned int	i;

	num = (n < 0 ? -n : n);
	dlina = ft_lenght(num);
	i = 0;
	if (!(chr = (char *)malloc(sizeof(char) * dlina + 1 + (n < 0 ? 1 : 0))))
		return (NULL);
	if (n < 0)
	{
		chr[i] = '-';
		dlina++;
	}
	i = dlina - 1;
	while (num > 9)
	{
		chr[i] = num % 10 + 48;
		num = num / 10;
		i--;
	}
	chr[i] = num % 10 + 48;
	chr[dlina] = '\0';
	return (chr);
}


