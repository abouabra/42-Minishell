/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 13:27:26 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/05 18:22:09 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	custom_exit(int exit_code)
{
	garbage_collector(NULL, 1);
	exit(exit_code);
}

void	garbage_collector(void *adress, int arg)
{
	static t_list	*head;
	t_list			*node;

	if (arg)
		ft_lstclear(&head, free);
	else
	{
		node = ft_lstnew(adress);
		ft_lstadd_back(&head, node);
	}
}

void	*my_alloc(size_t size)
{
	void	*str;

	str = malloc(size);
	if (!str)
		return (0);
	ft_memset(str, 0, size);
	garbage_collector(str, 0);
	return (str);
}
