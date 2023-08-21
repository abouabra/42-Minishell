/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 19:02:40 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 19:12:06 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	subshell_op_help(int n[SUB], int check, char **arr)
{
	if (g_vars->ex_status != 0
		&& g_vars->op[ft_strlen(g_vars->op) - 1] == '&')
	{
		n[nest_return] = skip_opertor(arr, n, check, "||");
		if (n[nest_return] != -2)
			return (n[nest_return]);
	}
	if (g_vars->ex_status == 0
		&& g_vars->op[ft_strlen(g_vars->op) - 1] == '|')
	{
		n[nest_return] = skip_opertor(arr, n, check, "&&");
		if (n[nest_return] != -2)
			return (n[nest_return]);
	}
	return (-2);
}

int	subshell_op(int n[SUB], int check, char **arr)
{
	if (n[par_count] == 1 && arr[n[i]][0] == '('
		&& arr[n[i]][ft_strlen(arr[n[i]]) - 1] == ')')
	{
		if (check && arr[n[i] -1] && (arr[n[i] -1][ft_strlen(
					arr[n[i] - 1]) - 1] == '&' || (arr[n[i] - 1][ft_strlen(
					arr[n[i] - 1]) - 1] == '|' && arr[n[i] - 1][ft_strlen(
				arr[n[i] - 1]) - 2] && arr[n[i] - 1][ft_strlen(
						arr[n[i] - 1]) - 2] == '|')))
		{
			if (subshell_op_help(n, check, arr) != -2)
				return (n[nest_return]);
		}
		if (!arr[n[i]])
			return (0);
		arr[n[i]] = ft_substr(arr[n[i]], 1, ft_strlen(arr[n[i]]) - 2);
		if (!arr[n[i]][0])
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	return (-2);
}

void	start_pipe(int n[SUB], char **arr)
{
	if (arr[n[i] + 1] && arr[n[i] + 1][0] && arr[n[i] + 1][0] == '|'
		&& arr[n[i] + 1][1] && arr[n[i] + 1][1] != '|')
		pipe(g_vars->next_pipefd);
	if (g_vars->prev_pipefd[1] != 0)
		close(g_vars->prev_pipefd[1]);
}

void	sub_piping(int n[SUB], char **arr)
{
	if (arr[n[i] + 1] && arr[n[i] + 1][0] && arr[n[i] + 1][0] == '|'
		&& arr[n[i] + 1][1] && arr[n[i] + 1][1] != '|')
	{
		dup2(g_vars->next_pipefd[1], 1);
		close(g_vars->next_pipefd[1]);
		close(g_vars->next_pipefd[0]);
	}
	if (n[i] > 0 && arr[n[i] - 1] && arr[n[i] - 1][ft_strlen(arr[n[i] - 1]) - 1]
		&& arr[n[i] - 1][ft_strlen(arr[n[i] - 1]) - 1] == '|'
		&& (!arr[n[i] - 1][ft_strlen(arr[n[i] - 1]) - 2]
		|| (arr[n[i] - 1][ft_strlen(arr[n[i] - 1]) - 2]
		&& arr[n[i] - 1][ft_strlen(arr[n[i] - 1]) - 2] != '|')))
	{
		dup2(g_vars->prev_pipefd[0], 0);
		close(g_vars->prev_pipefd[1]);
		close(g_vars->prev_pipefd[0]);
	}
}

void	sub_pipe(int n[SUB], char **arr, char **kobi, int check)
{
	int	pid;

	start_pipe(n, arr);
	pid = fork();
	if (pid == 0)
	{
		sub_piping(n, arr);
		nested_par(kobi, check, n[ind] + 1);
		exit(g_vars->ex_status);
	}
	else
	{
		if (waitpid(pid, &n[stat], 0) != -1)
			(g_vars->ex_status) = n[stat] >> 8;
		if (g_vars->prev_pipefd[0] != 0)
			close(g_vars->prev_pipefd[0]);
	}
}
