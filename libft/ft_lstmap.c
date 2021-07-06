/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 16:09:07 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/13 16:21:37 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*l;
	t_list	*st;

	l = ft_lstnew(f(lst->content));
	if (!lst || !l)
		return (NULL);
	st = l;
	while (lst->next)
	{
		lst = lst->next;
		l->next = ft_lstnew(f(lst->content));
		if (!l->next)
		{
			ft_lstclear(&st, del);
			return (NULL);
		}
		l = l->next;
	}
	return (st);
}
