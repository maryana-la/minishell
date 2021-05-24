/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 19:21:41 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/13 14:06:49 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		i;
	size_t		dlina;
	char		*temp;

	if (s == '\0')
		return (NULL);
	i = 0;
	temp = (char *)malloc((len + 1) * sizeof(char));
	if (temp == NULL)
		return (NULL);
	dlina = ft_strlen(s);
	if (start >= dlina)
	{
		temp[i] = '\0';
		return (temp);
	}
	while (i < len)
	{
		temp[i] = s[start + i];
		i++;
	}
	temp[i] = '\0';
	return (temp);
}
