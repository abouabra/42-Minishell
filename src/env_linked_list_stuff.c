/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_linked_list_stuff.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:41:16 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/26 22:14:01 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_last_env(t_env *lst)
{
	if (lst)
	{
		while (lst->next != NULL)
			lst = lst->next;
	}
	return (lst);
}

void	add_env_in_back(t_env **head, t_env *node)
{
	t_env	*tmp;

	if (*head)
	{
		tmp = ft_last_env(*head);
		tmp->next = node;
	}
	else
		*head = node;
}

void	add_env_in_front(t_env **head, t_env *node)
{
	if (*head && node)
		node->next = *head;
	if (node)
		*head = node;
}

t_env	*ft_new_env_node(char *env_id, char *env_data)
{
	t_env	*node;

	node = my_alloc(sizeof(t_env));
	if (!node)
		return (0);
	node->env_id = env_id;
	node->env_data = env_data;
	node->next = NULL;
	return (node);
}

void	ft_node_remove_if(t_env **head, char *env_id)
{
	t_env	*current;
	t_env	*prev;

	current = *head;
	prev = *head;
	if (current != NULL && !ft_strncmp(env_id, current->env_id, -1))
	{
		*head = current->next;
		current = NULL;
		return ;
	}
	while (current != NULL && ft_strncmp(env_id, current->env_id, -1))
	{
		prev = current;
		current = current->next;
	}
	if (current != NULL && !ft_strncmp(env_id, current->env_id, -1))
	{
		prev->next = current->next;
		current = NULL;
	}
}
