/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_command_linked_list.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:41:16 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/19 13:20:28 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd_redir	*ft_last_redir(t_cmd_redir *lst)
{
	if (lst)
	{
		while (lst->next != NULL)
			lst = lst->next;
	}
	return (lst);
}

void	add_redir_in_back(t_cmd_redir **head, t_cmd_redir *node)
{
	t_cmd_redir	*tmp;

	if (*head)
	{
		tmp = ft_last_redir(*head);
		tmp->next = node;
	}
	else
		*head = node;
}

t_cmd_redir	*ft_new_redir(int type, char *file)
{
	t_cmd_redir	*node;

	node = my_alloc(sizeof(t_cmd_redir));
	if (!node)
		return (0);
	node->type = type;
	node->file = file;
	node->next = NULL;
	return (node);
}
