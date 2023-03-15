/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 22:10:27 by ykhayri           #+#    #+#             */
/*   Updated: 2023/03/15 23:20:50 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *name)
{
	int		i;
	char	*built_in;
	char	**arr;

	built_in = "cd|echo|pwd|export|unset|env|exit";
	arr = ft_split(built_in, '|');
	i = -1;
	while (arr[++i])
		if (!ft_strncmp(name, arr[i], -1))
			return (1);
	return (0);
}

void	execution_phase(t_args *vars)
{
	t_command	*tmp;
	int			i;

	tmp = vars->command_head;
	i = -1;
	while (++i < vars->command_count)
	{
		if (is_built_in(tmp->command_args[0]) && ft_strncmp("echo",
				tmp->command_args[0], -1))
			execute_built_in(vars, tmp);
		else
			execute(vars, tmp, i);
		tmp = tmp->next;
	}
}

char	**convert_env_to_arr(t_env *head)
{
	char	**arr;
	char	*tmp;
	int		env_count;
	int		i;
	t_env	*env;

	env = head;
	env_count = ft_env_list_size(&head);
	arr = my_alloc((env_count + 1) * sizeof(char *));
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->env_id, "=");
		arr[i] = ft_strjoin(tmp, env->env_data);
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
