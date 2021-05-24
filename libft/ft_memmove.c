/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 21:09:49 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/14 00:09:41 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*temp_src;
	char	*temp_dst;
	size_t	i;

	if (src == dst)
		return (dst);
	temp_src = (char *)src;
	temp_dst = (char *)dst;
	if (src < dst)
	{
		i = len;
		while (i-- > 0)
			temp_dst[i] = temp_src[i];
	}
	else
	{
		i = 0;
		while (i < len)
		{
			temp_dst[i] = temp_src[i];
			i++;
		}
	}
	return (dst);
}
