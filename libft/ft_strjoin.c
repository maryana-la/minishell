/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 20:38:43 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/10 12:23:17 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	lenght;
	size_t	z;
	char	*temp;

	if (!s1 || !s2)
		return (NULL);
	lenght = ft_strlen(s1) + ft_strlen(s2);
	temp = (char *)malloc((lenght + 1) * sizeof(char));
	if (!(temp))
		return (NULL);
	z = 0;
	while (z < ft_strlen(s1))
	{
		temp[z] = s1[z];
		z++;
	}
	z = 0;
	while (z < ft_strlen(s2))
	{
		temp[ft_strlen(s1) + z] = s2[z];
		z++;
	}
	temp[lenght] = '\0';
	return (temp);
}
