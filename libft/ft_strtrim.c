/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 16:19:10 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/13 15:51:01 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_compare(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_first(const char *temp, const char *set)
{
	int	i;

	i = 0;
	while (temp[i])
	{
		if (ft_compare(temp[i], set) == 1)
			i++;
		else
			return (i);
	}
	return (i);
}

static int	ft_last(const char *temp, const char *set)
{
	int	i;

	i = ft_strlen(temp);
	while (i != 0)
	{
		if (ft_compare(temp[i - 1], set) == 1)
			i--;
		else
			return (i - 1);
	}
	return (i);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*ch_array;
	int		first;
	int		last;
	int		i;

	i = 0;
	if (s1 == NULL)
		return (NULL);
	first = ft_first(s1, set);
	last = ft_last(s1, set);
	if (last - first < 0)
		return (ft_substr("", 0, 0));
	ch_array = (char *)malloc((last - first + 1) + 1);
	if (!(ch_array))
		return (NULL);
	while (first <= last)
	{
		ch_array[i] = s1[first];
		i++;
		first++;
	}
	ch_array[i] = '\0';
	if (ch_array[0] == '\0')
		return (ch_array);
	return (ch_array);
}
