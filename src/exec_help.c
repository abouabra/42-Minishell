/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 22:10:27 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/03 02:34:12 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int built_in_should_execute_in_main(t_command *cmd)
{
	int		i;
	char	*built_in;
	char	**arr;
	if(!cmd->command_args[0])
		return 0;
	// if(!ft_strncmp(cmd->command_args[0], "cd", -1) && vars->command_count == 1)
	// 	return 1;
	if(!ft_strncmp(cmd->command_args[0], "export", -1) && cmd->command_args[1])
		return 1;
	built_in = "cd|unset|exit";
	arr = ft_split(built_in, '|');
	i = -1;
	while (arr[++i])
	{
		if (!ft_strncmp(cmd->command_args[0], arr[i], -1))
			return (1);

	}
	return (0);
}

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
