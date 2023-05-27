/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 12:03:37 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/27 21:33:16 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **split_arg(char *arg)
{
	char **arr;

	arr = my_alloc(3 * sizeof(char *));
	if(!arr)
		return 0;
	int i=-1;
	while(arg[++i])
	{
		if(arg[i] == '=')
		{
			arr[0] = ft_substr(arg, 0, i);
			arr[1] = ft_substr(arg, i + 1, ft_strlen(arg));
			if(!arr[1])
				arr[1] = "";
			arr[2] = 0;
			return arr;
		}
	}
	return 0;
}

void	env_export(t_command *command)
{
	t_env	*new_env;
	t_env	*search;
	t_env	*env;
	char	**args;

	if (!command->command_args[1])
	{
		env = vars->env_head;
		while (env)
		{
			printf("declare -x %s=\"%s\"\n", env->env_id, env->env_data);
			env = env->next;
		}
		return;
	}
	args = split_arg(command->command_args[1]);
	if(!args)
	{
		new_env = ft_new_env_node(command->command_args[1], "");
		new_env->not_declared = 1;
		add_env_in_back(&vars->env_head, new_env);
		return;
	}
	search = vars->env_head;
	the_search_env(&search, args);
	if (!search)
	{
		if(!args[1])
			new_env = ft_new_env_node(args[0], "");
		else
			new_env = ft_new_env_node(args[0], ft_strtrim(args[1], "\'\""));
		add_env_in_back(&vars->env_head, new_env);
	}
}

void	env()
{
	t_env	*env;

	env = vars->env_head;
	while (env)
	{
		if(!env->not_declared)
		{
			char *tmp = env->env_data;
			if(!tmp)
				printf("%s=\n", env->env_id);
			else
				printf("%s=%s\n", env->env_id, env->env_data);
		}
		env = env->next;
	}	
}

void	unset(char *env_id)
{
	t_env	*env;

	env = vars->env_head;
	while (env)
	{
		if (!ft_strncmp(env_id, env->env_id, -1))
			break ;
		env = env->next;
	}
	ft_node_remove_if(&vars->env_head, env_id);
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
