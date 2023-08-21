/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_phase.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:08:47 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/19 21:42:42 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_built_in(t_command *command)
{
	if (!ft_strncmp("cd", command->command_args[0], -1))
		cd(command);
	else if (!ft_strncmp("exit", command->command_args[0], -1))
		my_exit(command);
	else if (!ft_strncmp("export", command->command_args[0], -1))
		env_export(command);
	else if (!ft_strncmp("unset", command->command_args[0], -1))
		unset(command);
}

void	builtin_should_execute_in_child2(t_command *tmp)
{
	if (!ft_strncmp("pwd", tmp->command_args[0], -1))
	{
		pwd();
		custom_exit(0);
	}
	if (!ft_strncmp("cd", tmp->command_args[0], -1))
	{
		cd(tmp);
		custom_exit(0);
	}
}

int	builtin_should_execute_in_child(t_command *tmp)
{
	if (!ft_strncmp("echo", tmp->command_args[0], -1))
	{
		echo(tmp);
		custom_exit(0);
	}
	if (!ft_strncmp("env", tmp->command_args[0], -1))
	{
		env();
		custom_exit(0);
	}
	if (!ft_strncmp("export", tmp->command_args[0], -1))
	{
		env_export(tmp);
		custom_exit(0);
	}
	builtin_should_execute_in_child2(tmp);
	return (0);
}

static void	handle_child3(t_command *tmp)
{
	if (!builtin_should_execute_in_child(tmp))
	{
		execve(tmp->command_path, tmp->command_args,
			convert_env_to_arr(g_vars->env_head));
		custom_exit(1);
	}
}

void	handle_child(t_command *tmp, int i)
{
	if ((g_vars->pipe == 2 || g_vars->pipe == 3) || (i > 0 && (!g_vars->op[0]
				|| (g_vars->op[0] && g_vars->op[(i - 1) * 2] == '1'))))
	{
		dup2(g_vars->prev_pipefd[0], 0);
		close(g_vars->prev_pipefd[0]);
		close(g_vars->prev_pipefd[1]);
	}
	if ((g_vars->pipe == 1 || g_vars->pipe == 3) || (i < g_vars->command_count
			- 1 && (!g_vars->op[0] || (g_vars->op[0] && g_vars->op[i
						* 2] == '1'))))
	{
		dup2(g_vars->next_pipefd[1], 1);
		close(g_vars->next_pipefd[0]);
		close(g_vars->next_pipefd[1]);
	}
	do_redirections(tmp->redir);
	if (tmp->is_valid_command == 0)
		custom_exit(127);
	if (tmp->is_valid_command == 69)
		custom_exit(0);
	handle_child3(tmp);
}
