/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_phase.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:08:47 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 18:37:18 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_built_in(t_args *vars, t_command *command)
{
	if (!ft_strncmp("cd", command->command_args[0], -1))
		cd(vars, command);
	if (!ft_strncmp("pwd", command->command_args[0], -1))
		pwd();
	if (!ft_strncmp("env", command->command_args[0], -1))
		env(vars);
	if (!ft_strncmp("exit", command->command_args[0], -1))
		my_exit(command->command_args[1]);
	if (!ft_strncmp("export", command->command_args[0], -1))
		env_export(vars, command);
	if (!ft_strncmp("unset", command->command_args[0], -1))
		unset(vars, command->command_args[1]);
}

static void	handle_child3(t_args *vars, t_command *tmp)
{
	if (!ft_strncmp("echo", tmp->command_args[0], -1))
	{
		echo(tmp);
		custom_exit(0);
	}
	else if (!ft_strncmp("env", tmp->command_args[0], -1))
	{
		env(vars);
		custom_exit(0);
	}
	else
	{
		execve(tmp->command_path, tmp->command_args,
			convert_env_to_arr(vars->env_head));
		custom_exit(1);
	}
}

static void	handle_child2(t_args *vars, t_command *tmp, int fd)
{
	if (tmp->is_output)
	{
		if (tmp->is_append)
			fd = open(tmp->append_file, O_RDWR | O_APPEND | O_CREAT, 0644);
		else
			fd = open(tmp->output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
			custom_exit(1);
		dup2(fd, 1);
		close(fd);
	}
	if (tmp->is_herdoc)
	{
		fd = open("/tmp/herdoc_data", O_WRONLY | O_TRUNC | O_CREAT, 0644);
		write(fd, tmp->herdoc_data, ft_strlen(tmp->herdoc_data));
		close(fd);
		fd = open("/tmp/herdoc_data", O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	handle_child3(vars, tmp);
}

void	handle_child(t_args *vars, t_command *tmp, int fd, int i)
{
	if (tmp->is_valid_command == 0)
		custom_exit(127);
	if (tmp->is_valid_command == 69)
		custom_exit(0);
	if (i > 0)
	{
		dup2(vars->prev_pipefd[0], 0);
		close(vars->prev_pipefd[0]);
		close(vars->prev_pipefd[1]);
	}
	if (i < vars->command_count - 1)
	{
		dup2(vars->next_pipefd[1], 1);
		close(vars->next_pipefd[0]);
		close(vars->next_pipefd[1]);
	}
	if (tmp->is_input)
	{
		fd = open(tmp->input_file, O_RDONLY);
		if (fd == -1)
			custom_exit(1);
		dup2(fd, 0);
		close(fd);
	}
	handle_child2(vars, tmp, fd);
}
