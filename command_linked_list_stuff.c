/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_linked_list_stuff.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:41:16 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/06 19:39:19 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	node->is_input = info->is_input;
	node->input_file = info->input_file;
	node->is_output = info->is_output;
	node->output_file = info->output_file;
	node->is_append = info->is_append;
	node->append_file = info->append_file;
	node->is_herdoc = info->is_herdoc;
	node->herdoc_data = info->herdoc_data;
	node->herdoc_limiter = info->herdoc_limiter;
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
