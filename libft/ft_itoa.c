/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 16:07:40 by jjacquel          #+#    #+#             */
/*   Updated: 2021/06/29 18:41:10 by jjacquel         ###   ########.fr       */
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

int	znak	(int n)
{
	int	num;

	if (n < 0)
		num = -n;
	else
		num = n;
	return (num);
}

char	*malloc_chr (unsigned int dlina, int n)
{
	char	*chr;

	if (n < 0)
		chr = (char *)malloc(sizeof(char) * dlina + 2);
	else
		chr = (char *)malloc(sizeof(char) * dlina + 1);
	return (chr);
}

char	*ft_itoa(int n)
{
	char			*chr;
	unsigned int	dlina;
	unsigned int	num;
	unsigned int	i;

	num = znak(n);
	dlina = ft_lenght(num);
	chr = malloc_chr(dlina, n);
	i = 0;
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
