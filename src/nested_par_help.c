/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_par_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 19:00:52 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 19:11:52 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	not_subshell_pieps2(int n[SUB], char **arr, int check)
{
	if ((check && n[i] - 1 >= 0 && arr[n[i]][0] && arr[n[i]][0] == '|'
		&& (!arr[n[i]][1] || (arr[n[i]][1] && arr[n[i]][1] != '|')))
		&& (check && arr[n[i] + 1] && par_coount(arr[n[i] + 1]) == 1
		&& arr[n[i] + 1][0] == '(' && arr[n[i] + 1][ft_strlen(
			arr[n[i] + 1]) - 1] == ')' && arr[n[i]][ft_strlen(arr[n[i]]) - 1]
			&& arr[n[i]][ft_strlen(arr[n[i]]) - 1] == '|'
			&& (!arr[n[i]][ft_strlen(arr[n[i]]) - 2]
			|| (arr[n[i]][ft_strlen(arr[n[i]]) - 2]
			&& arr[n[i]][ft_strlen(arr[n[i]]) - 2] != '|')))
	)
	{
		g_vars->pipe = 3;
		if (ft_strlen(arr[n[i]]) < 2)
			g_vars->pipe = 0;
		g_vars->prev_pipefd[0] = g_vars->next_pipefd[0];
		g_vars->prev_pipefd[1] = g_vars->next_pipefd[1];
	}
}

void	not_subshell_pieps(int n[SUB], char **arr, int check)
{
	if (check && n[i] - 1 >= 0 && arr[n[i]][0] && arr[n[i]][0] == '|'
		&& (!arr[n[i]][1] || (arr[n[i]][1] && arr[n[i]][1] != '|')))
	{
		g_vars->pipe = 2;
		g_vars->prev_pipefd[0] = g_vars->next_pipefd[0];
		g_vars->prev_pipefd[1] = g_vars->next_pipefd[1];
	}
	if (check && arr[n[i] + 1] && par_coount(arr[n[i] + 1]) == 1
		&& arr[n[i] + 1][0] == '(' && arr[n[i] + 1][ft_strlen(
			arr[n[i] + 1]) - 1] == ')' && arr[n[i]][ft_strlen(arr[n[i]]) - 1]
			&& arr[n[i]][ft_strlen(arr[n[i]]) - 1] == '|'
			&& (!arr[n[i]][ft_strlen(arr[n[i]]) - 2]
			|| (arr[n[i]][ft_strlen(arr[n[i]]) - 2] && arr[n[i]][ft_strlen(
			arr[n[i]]) - 2] != '|')))
		g_vars->pipe = 1;
	not_subshell_pieps2(n, arr, check);
}

void	skip_sub_array(char op, int n[SUB], char **arr)
{
	while (arr[n[i]][++n[s]])
	{
		if (arr[n[i]][n[s] + 1] && arr[n[i]][n[s]] == op
			&& arr[n[i]][n[s] + 1] == op)
		{
			if (arr[n[i]][n[s] + 2])
				n[s] += 2;
			break ;
		}
	}
}

int	not_subshell_skip_help(char **arr, int check, int n[SUB], char op)
{
	if (n[s] == -1)
	{
		n[s] = 2;
		n[success] = 1;
	}
	arr[n[i]] = ft_substr(arr[n[i]], n[s], -1);
	if (!arr[n[i]][0])
	{
		if (n[success] && arr[n[i] + 1])
			return (nested_par(&arr[n[i] + 1], check, n[ind] +1));
		else if (!n[success])
		{
			while (arr[n[i]] && ft_strtrim(arr[n[i]], " \t")[0] != op)
				n[i]++;
			if (arr[n[i]])
			{
				if (ft_strtrim(arr[n[i]], " \t")[0] == op
					&& ft_strlen(ft_strtrim(arr[n[i]], " \t")) == 2)
					n[i]++;
				return (nested_par(split_par(arr[n[i]]), check, n[ind] + 1));
			}
		}
	}
	return (-2);
}

int	not_subshell_skip(int n[SUB], char **arr, int check)
{
	char	op;
	int		return_val;

	wait_pid(n, &op);
	arr[n[i]] = ft_strtrim(arr[n[i]], " \t");
	if (arr[n[i]][0] == '&' || (arr[n[i]][0] == '|'
		&& arr[n[i]][1] && arr[n[i]][1] == '|'))
	{
		n[s] = -1;
		if (g_vars->ex_status == 0 && arr[n[i]][0] == '|')
		{
			op = '&';
			skip_sub_array(op, n, arr);
		}
		else if (g_vars->ex_status != 0 && arr[n[i]][0] == '&')
		{
			op = '|';
			skip_sub_array(op, n, arr);
		}
		n[success] = 0;
		return_val = not_subshell_skip_help(arr, check, n, op);
		if (return_val != -2)
			return (return_val);
	}
	return (-2);
}
