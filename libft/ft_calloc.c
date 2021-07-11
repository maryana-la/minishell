/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 20:59:57 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/03 23:18:08 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	char	*p;

	i = 0;
	p = malloc(count * size);
	if (p == NULL)
		return (NULL);
	while (i < (size * count))
	{
		p[i] = 0;
		i++;
	}
	return ((void *)p);
}
