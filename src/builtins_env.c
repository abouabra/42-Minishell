/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 12:03:37 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 11:38:18 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**split_arg(char *arg)
{
	char	**arr;
	int		i;

	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '=')
		{
			arr = my_alloc(3 * sizeof(char *));
			arr[0] = ft_substr(arg, 0, i);
			arr[1] = ft_substr(arg, i + 1, ft_strlen(arg));
			if (!arr[1])
				arr[1] = "";
			arr[2] = 0;
			return (arr);
		}
	}
	arr = my_alloc(2 * sizeof(char *));
	arr[0] = arg;
	arr[1] = NULL;
	return (arr);
}

int	analyze_args(char **arg)
{
	int	j;

	if (arg[0])
	{
		j = -1;
		while (arg[0][++j])
		{
			if (arg[0][j] == '_')
				j++;
			if (arg[0][j] && ft_isdigit(arg[0][j]) && j == 0)
				return (0);
			if (arg[0][j] && (arg[0][j] == '@' || arg[0][j] == '.'))
				return (0);
		}
	}
	return (1);
}

void	env(void)
{
	t_env	*env;
	char	*tmp;

	env = g_vars->env_head;
	while (env)
	{
		if (env->not_declared != no_value)
		{
			tmp = env->env_data;
			if (!tmp)
				ft_dprintf(1, "%s=\n", env->env_id);
			else
				ft_dprintf(1, "%s=%s\n", env->env_id, env->env_data);
		}
		env = env->next;
	}
}

void	unset(t_command *cmd)
{
	t_env	*env;
	int		i;

	i = 0;
	while (cmd->command_args[++i])
	{
		env = g_vars->env_head;
		while (env)
		{
			if (!ft_strncmp(cmd->command_args[i], env->env_id, -1))
				break ;
			env = env->next;
		}
		ft_node_remove_if (&g_vars->env_head, cmd->command_args[i]);
	}
}

void	the_search_env(t_env **search, char **args)
{
	while (*search)
	{
		if (!ft_strncmp((*search)->env_id, args[0], -1))
		{
			(*search)->env_data = ft_strtrim(args[1], "\'\"");
			break ;
		}
		(*search) = (*search)->next;
	}
}
