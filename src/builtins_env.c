/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 12:03:37 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/27 23:55:36 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **split_arg(char *arg)
{
	char **arr;

	int i=-1;
	while(arg[++i])
	{
		if(arg[i] == '=')
		{
			arr = my_alloc(3 * sizeof(char *));
			arr[0] = ft_substr(arg, 0, i);
			arr[1] = ft_substr(arg, i + 1, ft_strlen(arg));
			if(!arr[1])
				arr[1] = "";
			arr[2] = 0;
			return arr;
		}
	}
	arr = my_alloc(2 * sizeof(char *));
	arr[0] = arg;
	arr[1] = NULL;
	return arr;
}

int analyze_args(char **arg)
{
	int i = -1;

	while(arg[++i])
	{
		int j = -1;
		while(arg[i][++j])
		{
			if(arg[i][j] == '_')
				j++;
			if(arg[i][j] && ft_isdigit(arg[i][j]) && j==0)
				return 0;
			if(arg[i][j] && arg[i][j] == '@')
				return 0;
		}
	}
	return 1;
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
			char *tmp = env->env_data;
			if(env->not_declared == no_value && !tmp[0])
				ft_dprintf(1,"declare -x %s\n", env->env_id);
			else
			{
				int j = -1;
				ft_dprintf(1,"declare -x %s=\"", env->env_id);
				while(tmp[++j])
				{
					if(tmp[j] == '\"' || tmp[j] == '$' || tmp[j] == '\\')
						ft_dprintf(1,"\\");
					ft_dprintf(1,"%c", tmp[j]);
				}
				ft_dprintf(1,"\"\n");
			}
			env = env->next;
		}
		return;
	}
	int		i = 0;
	while(command->command_args[++i])
	{
		args = split_arg(command->command_args[i]);
		if(!analyze_args(args))
		{
			ft_dprintf(2,"minishell: not a valid identifier\n");
			*vars->ex_status = 1;
		}
		else
		{	
			if(!args[1])
			{
				new_env = ft_new_env_node(command->command_args[i], "");
				new_env->not_declared = no_value;
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
				new_env->not_declared = user_defined;
				add_env_in_back(&vars->env_head, new_env);
			}
		}
	}
}

void	env()
{
	t_env	*env;

	env = vars->env_head;
	while (env)
	{
		if(env->not_declared != no_value)
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

void	unset(t_command *cmd)
{
	t_env	*env;

	int i = 0;
	while(cmd->command_args[++i])
	{
		env = vars->env_head;
		while (env)
		{
			if (!ft_strncmp(cmd->command_args[i], env->env_id, -1))
				break ;
			env = env->next;
		}
		ft_node_remove_if(&vars->env_head, cmd->command_args[i]);
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
