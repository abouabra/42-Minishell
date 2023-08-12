/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_linked_list_stuff.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:41:16 by abouabra          #+#    #+#             */
/*   Updated: 2023/06/01 21:09:35 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_command	*ft_last_command(t_command *lst)
{
	if (lst)
	{
		while (lst->next != NULL)
			lst = lst->next;
	}
	return (lst);
}

void	add_command_in_back(t_command **head, t_command *node)
{
	t_command	*tmp;

	if (*head)
	{
		tmp = ft_last_command(*head);
		tmp->next = node;
	}
	else
		*head = node;
}

void	add_command_in_front(t_command **head, t_command *node)
{
	if (*head && node)
		node->next = *head;
	if (node)
		*head = node;
}

t_command	*ft_new_command(t_fill_info *info)
{
	t_command	*node;

	node = my_alloc(sizeof(t_command));
	if (!node)
		return (0);
	node->command_path = info->command_path;
	node->command_args = info->command_args;
	node->quote_type = info->quote_type;
	node->is_valid_command = info->is_valid_command;
	node->info = my_alloc(sizeof(t_fill_info));
	node->info = ft_memmove(node->info,info, sizeof(t_fill_info));
	node->redir = info->redir;
	node->next = NULL;
	return (node);
}

int	ft_command_list_size(t_command **head)
{
	t_command	*tmp;
	int			i;

	i = 0;
	tmp = *head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
