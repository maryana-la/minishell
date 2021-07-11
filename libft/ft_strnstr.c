/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 14:04:35 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/11 02:06:28 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	z;
	size_t	i;

	if (!*needle)
		return ((char *)haystack);
	z = 0;
	while (haystack[z] != '\0' && (size_t)z < len)
	{
		if (haystack[z] == needle[0])
		{
			i = 1;
			while (needle[i] != '\0' && haystack[z + i] == \
			needle[i] && (size_t)(z + i) < len)
				++i;
			if (needle[i] == '\0')
				return ((char *)&haystack[z]);
		}
		++z;
	}
	return (0);
}
