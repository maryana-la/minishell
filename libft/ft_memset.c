/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 10:30:43 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/03 16:19:28 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t n)
{
	size_t		i;
	char		*temp;

	temp = dest;
	i = 0;
	while (i < n)
	{
		temp[i] = c;
		i++;
	}
	return (dest);
}
