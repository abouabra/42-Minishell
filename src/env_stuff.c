/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:33:03 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/30 17:27:58 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_data(char *id)
{
	t_env	*env;

	env = vars->env_head;
	while (env)
	{
		if (!ft_strncmp(id, env->env_id, -1))
			return (env->env_data);
		env = env->next;
	}
	return (NULL);
}

void	set_env()
{
	t_env	*env;
	char	**arr;
	int		i;

	i = -1;
	while (vars->ev[++i])
	{
		arr = split_arg(vars->ev[i]);
		env = ft_new_env_node(arr[0], arr[1]);
		add_env_in_back(&vars->env_head, env);
	}	
}

int	ft_env_list_size(t_env **head)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = *head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
