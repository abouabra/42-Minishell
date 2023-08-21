/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:11:02 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 20:10:28 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	doub_sin_skip(int *sin, int *doub, char *s, int i)
{
	if (s[i] == '\'')
		if (!*(doub))
			*(sin) = !*(sin);
	if (s[i] == '"')
		if (!*(sin))
			*(doub) = !*(doub);
}

char	*are_two_args_number(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[++i])
	{
		j = -1;
		while (str[i][++j])
		{
			if (str[i][j] > '9' || str[i][j] < '0')
				return (&str[i][j]);
		}
	}
	return (0);
}

char	*is_arg_number(char *str)
{
	int	j;

	j = -1;
	while (str[++j])
	{
		if (str[j] == '-' && str[j + 2] == '\0')
			return (0);
		if (str[j] > '9' || str[j] < '0')
			return (&str[j]);
	}
	return (0);
}

void	fd_handler(int i)
{
	if ((g_vars->pipe == 2 || g_vars->pipe == 3)
		|| (i > 0 && (!g_vars->op[0] || (g_vars->op[0]
					&& g_vars->op[(i - 1) * 2] == '1'))))
	{
		if (g_vars->pipe == 2)
			g_vars->pipe = 0;
		close(g_vars->prev_pipefd[0]);
		close(g_vars->prev_pipefd[1]);
	}
	if ((g_vars->pipe == 1 || g_vars->pipe == 2 || g_vars->pipe == 3)
		|| (i < g_vars->command_count - 1 && (!g_vars->op[0]
				|| (g_vars->op[0] && g_vars->op[i * 2] == '1'))))
	{
		g_vars->pipe = 0;
		g_vars->prev_pipefd[0] = g_vars->next_pipefd[0];
		g_vars->prev_pipefd[1] = g_vars->next_pipefd[1];
	}
}

void	execution_phase(void)
{
	t_command	*tmp;
	int			status;

	g_vars->pid = my_alloc(sizeof(int) * g_vars->command_count);
	tmp = g_vars->command_head;
	start_execution(tmp);
	tmp = g_vars->command_head;
	start_waiting(tmp, &status);
	if (g_vars->is_interrupted)
	{
		if (g_vars->interrupted_mode == 1)
			g_vars->ex_status = 130;
		if (g_vars->interrupted_mode == 2)
			g_vars->ex_status = 131;
		if (g_vars->interrupted_mode == 3)
			g_vars->ex_status = 1;
		g_vars->is_interrupted = 0;
		g_vars->interrupted_mode = 0;
	}
}
