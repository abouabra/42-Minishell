/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:33:03 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 11:38:52 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_data(char *id)
{
	t_env	*env;

	env = g_vars->env_head;
	while (env)
	{
		if (!ft_strncmp(id, env->env_id, -1))
			return (env->env_data);
		env = env->next;
	}
	return (NULL);
}

void	set_env(void)
{
	t_env	*env;
	char	**arr;
	int		i;

	i = -1;
	while (g_vars->ev[++i])
	{
		arr = split_arg(g_vars->ev[i]);
		env = ft_new_env_node(arr[0], arr[1]);
		add_env_in_back(&g_vars->env_head, env);
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

void	my_exit2(t_command *command, int *status)
{
	if (is_arg_number(command->command_args[1]))
	{
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
			is_arg_number(command->command_args[1]));
		*status = 255;
	}
	else
		*status = ft_atoi(command->command_args[1]);
}

void	my_exit(t_command *command)
{
	int	status;

	status = 0;
	if (!command->command_args[1])
		status = g_vars->ex_status;
	else if (command->command_args[1] && command->command_args[2])
	{
		if ((is_arg_number(command->command_args[1])
				&& !is_arg_number(command->command_args[2]))
			|| (is_arg_number(command->command_args[1])
				&& is_arg_number(command->command_args[2])))
		{
			status = 255;
			ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
				are_two_args_number(command->command_args));
		}
		else
		{
			status = 1;
			ft_dprintf(2, "minishell: exit: too many arguments\n");
		}
	}
	else
		my_exit2(command, &status);
	custom_exit(status);
}
