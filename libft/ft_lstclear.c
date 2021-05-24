/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 18:06:14 by jjacquel          #+#    #+#             */
/*   Updated: 2020/11/11 18:15:17 by jjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;

	if (del == NULL)
		return ;
	while (*lst != NULL)
	{
		del((*lst)->content);
		temp = *lst;
		*lst = temp->next;
		free(temp);
	}
	*lst = NULL;
}
