/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 12:22:45 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/13 15:14:54 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_lenght(const char *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[i] == c)
		i++;
	while (s[i] != c && s[i++] != '\0')
		len++;
	return (len);
}

static int	ft_totalwords(const char *s, char c)
{
	int	i;
	int	z;

	i = 0;
	z = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
			z++;
		while (s[i] != c && s[i + 1])
			i++;
		i++;
	}
	return (z);
}

static char	**ft_free(char **temp, int i)
{
	while (--i > 0)
	{
		free(temp[i]);
	}
	free(temp);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**temp;
	int		i;
	int		n;
	int		j;

	j = 0;
	i = 0;
	temp = (char **)malloc(sizeof(char *) * (ft_totalwords(s, c) + 1));
	if (!s || !(temp))
		return (NULL);
	while (i < ft_totalwords(s, c))
	{
		n = 0;
		temp[i] = (char *)malloc(sizeof(char) * (ft_lenght(&s[j], c) + 1));
		if (!(temp[i]))
			return (ft_free(temp, i));
		while (s[j] == c)
			j++;
		while (s[j] != c && s[j])
			temp[i][n++] = s[j++];
		temp[i][n] = '\0';
		i++;
	}
	temp[i] = NULL;
	return (temp);
}
