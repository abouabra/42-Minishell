/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:09:05 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 20:14:12 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_op_help(int *i, t_command **tmp)
{
	if (g_vars->ex_status == 0 && g_vars->op[((*i) - 1) * 2 + 1] == '|')
	{
		while (g_vars->op[((*i) - 1) * 2] && ((g_vars->op[((*i) - 1) * 2] == '1'
					&& g_vars->op[((*i) - 1) * 2 + 1] == '|')
				|| (g_vars->op[((*i) - 1) * 2] == '2'
					&& g_vars->op[((*i) - 1) * 2 + 1] != '&')))
		{
			(*i)++;
			(*tmp) = (*tmp)->next;
		}
	}
}

void	execute_op(int *i, t_command **tmp)
{
	int	j;
	int	status;

	j = -1;
	while (++j < (*i))
	{
		if (waitpid(g_vars->pid[j], &status, 0) != -1)
		{
			(g_vars->ex_status) = status >> 8;
		}
	}
	if (g_vars->ex_status != 0 && g_vars->op[((*i) - 1) * 2 + 1] == '&')
	{
		while (g_vars->op[((*i) - 1) * 2] && ((g_vars->op[((*i) - 1) * 2] == '1'
					&& g_vars->op[((*i) - 1) * 2 + 1] == '|')
				|| (g_vars->op[((*i) - 1) * 2] == '2'
					&& g_vars->op[((*i) - 1) * 2 + 1] != '|')))
		{
			(*i)++;
			(*tmp) = (*tmp)->next;
		}
	}
	execute_op_help(i, tmp);
}

void	execute_help(t_command **tmp, t_cmd_redir *redir)
{
	while (redir)
	{
		if (redir->type == OUTPUT)
			check_permision(NULL, redir->file, 3);
		else if (redir->type == INPUT)
			check_permision(NULL, redir->file, 2);
		else if (redir->type == APPEND)
			check_permision(NULL, redir->file, 3);
		redir = redir->next;
	}
	if ((*tmp)->command_args && (*tmp)->command_args[0])
	{
		if (check_permision((*tmp)->command_path, (*tmp)->command_args[0], 1))
			(*tmp)->is_valid_command = 0;
		else
			(*tmp)->is_valid_command = 1;
	}
	if ((g_vars->pipe == 1 || g_vars->pipe == 3)
		|| (i < g_vars->command_count - 1
			&& (!g_vars->op[0] || (g_vars->op[0] && g_vars->op[i * 2] == '1'))))
	{
		pipe(g_vars->next_pipefd);
	}
}

void	execute(t_command **tmp, int *index)
{
	int			i;
	t_cmd_redir	*redir;

	i = *index;
	if (i > 0 && g_vars->op[(i - 1) * 2] == '2')
	{
		execute_op(&i, tmp);
		*index = i;
		if (i == g_vars->command_count)
			return ;
	}
	redir = (*tmp)->redir;
	execute_help(tmp, redir);
	g_vars->pid[i] = fork();
	if (g_vars->pid[i] == -1)
		return ;
	if (g_vars->pid[i] == 0)
		handle_child((*tmp), i);
	if ((i == 0 && g_vars->op[i * 2] == '1')
		|| (i > 0 && (g_vars->op[(i - 1) * 2] == '1'
				|| g_vars->op[(i) * 2] == '1')))
	{
		fd_handler(i);
	}
}

void	start_waiting(t_command *tmp, int *status)
{
	int	i;
	int	j;

	i = -1;
	while (++i < g_vars->command_count)
	{
		if (!built_in_should_execute_in_main(tmp)
			|| (built_in_should_execute_in_main(tmp)
				&& ((g_vars->op[0] && ((i - 1 >= 0
								&& g_vars->op[(i - 1) * 2] == '1')
							|| (g_vars->op[i * 2]
								&& g_vars->op[i * 2] == '1'))))))
		{
			j = -1;
			while (++j <= i)
				if (waitpid(g_vars->pid[j], status, 0) != -1)
					(g_vars->ex_status) = (*status) >> 8;
		}
		tmp = tmp->next;
	}
}
