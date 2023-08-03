/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_phase.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:08:47 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/02 23:08:55 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

void	execute_built_in(t_command *command)
{
	if (!ft_strncmp("cd", command->command_args[0], -1))
		cd(command);
	else if (!ft_strncmp("exit", command->command_args[0], -1))
		my_exit(command);
	else if (!ft_strncmp("export", command->command_args[0], -1))
		env_export(command);
	else if (!ft_strncmp("unset", command->command_args[0], -1))
		unset( command);
}

int	builtin_should_execute_in_child(t_command *tmp)
{
	if (!ft_strncmp("echo", tmp->command_args[0], -1))
	{
		echo(tmp);
		custom_exit(0);
	}
	else if (!ft_strncmp("env", tmp->command_args[0], -1))
	{
		env();
		custom_exit(0);
	}
	else if (!ft_strncmp("export", tmp->command_args[0], -1))
	{
		env_export(tmp);
		custom_exit(0);
	}
	else if (!ft_strncmp("pwd", tmp->command_args[0], -1))
	{
		pwd();
		custom_exit(0);
	}
	else if (!ft_strncmp("cd", tmp->command_args[0], -1))
	{
		cd(tmp);
		custom_exit(0);
	}
	return 0;
}

static void	handle_child3(t_command *tmp)
{
	if(!builtin_should_execute_in_child(tmp))
	{
		execve(tmp->command_path, tmp->command_args,
			convert_env_to_arr(vars->env_head));
		custom_exit(1);
	}
}

void do_redirections(t_cmd_redir *head)
{

	// t_cmd_redir * tmp = vars->command_head->redir;
	// while(tmp)
	// {
	// 	printf("type: %d   file name: %s\n", tmp->type, tmp->file);
	// 	tmp = tmp->next;
	// }
	
	t_cmd_redir *redir = head;
	int fd;
	while(redir)
	{
		if(redir->type == INPUT)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 0);
			close(fd);
		}
		else if(redir->type == OUTPUT)
		{
			fd = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 1);
			close(fd);
		}
		else if(redir->type == APPEND)
		{
			fd = open(redir->file, O_RDWR | O_APPEND | O_CREAT, 0644);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 1);
			close(fd);
		}
		else if(redir->type == HEREDOC)
		{
			fd = open("/tmp/herdoc_data", O_RDONLY);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 0);
			close(fd);
		}
		redir = redir->next;
	}
}

void	handle_child(t_command *tmp, int i)
{
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
	do_redirections(tmp->redir);
	if (tmp->is_valid_command == 0)
		custom_exit(127);
	if (tmp->is_valid_command == 69)
		custom_exit(0);
	handle_child3(tmp);
}
