/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 15:24:17 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/19 20:37:50 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env_export3(t_command *command, char **args, int i)
{
	t_env	*new_env;
	t_env	*search;

	if (!args[1])
	{
		new_env = ft_new_env_node(command->command_args[i], "");
		new_env->not_declared = no_value;
		add_env_in_back(&g_vars->env_head, new_env);
		return (0);
	}
	search = g_vars->env_head;
	the_search_env(&search, args);
	if (!search)
	{
		if (!args[1])
			new_env = ft_new_env_node(args[0], "");
		else
			new_env = ft_new_env_node(args[0],
					ft_strtrim(args[1], "\'\""));
		new_env->not_declared = user_defined;
		add_env_in_back(&g_vars->env_head, new_env);
	}
	return (1);
}

void	env_export2(void)
{
	char	*tmp;
	t_env	*env;
	int		j;

	env = g_vars->env_head;
	while (env)
	{
		tmp = env->env_data;
		if (env->not_declared == no_value && !tmp[0])
			ft_dprintf(1, "declare -x %s\n", env->env_id);
		else
		{
			j = -1;
			ft_dprintf(1, "declare -x %s=\"", env->env_id);
			while (tmp[++j])
			{
				if (tmp[j] == '\"' || tmp[j] == '$' || tmp[j] == '\\')
					ft_dprintf(1, "\\");
				ft_dprintf(1, "%c", tmp[j]);
			}
			ft_dprintf(1, "\"\n");
		}
		env = env->next;
	}
}

void	env_export(t_command *command)
{
	char	**args;
	int		i;

	if (!command->command_args[1])
	{
		env_export2();
		return ;
	}
	i = 0;
	while (command->command_args[++i])
	{
		args = split_arg(command->command_args[i]);
		if (!analyze_args(args))
		{
			ft_dprintf(2, "minishell: not a valid identifier\n");
			g_vars->ex_status = 1;
		}
		else
			if (!env_export3(command, args, i))
				return ;
	}
}
