/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 21:08:09 by jjacquel          #+#    #+#             */
/*   Updated: 2021/07/06 15:20:15 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*temp_dst;
	char		*temp_src;
	size_t		i;

	if (dst == NULL && src == NULL)
		return (NULL);
	temp_dst = (char *)dst;
	temp_src = (char *)src;
	i = 0;
	while (i < n)
	{
		temp_dst[i] = temp_src[i];
		i++;
	}
	return (dst);
}
