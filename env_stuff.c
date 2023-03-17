/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:33:03 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/06 19:58:29 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_data(t_args *vars, char *id)
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

void	set_env(t_args *vars)
{
	t_env	*env;
	char	**arr;
	int		i;

	i = -1;
	while (vars->ev[++i])
	{
		arr = ft_split(vars->ev[i], '=');
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
