/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_par.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 18:57:54 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 20:27:23 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_pid(int n[SUB], char *op)
{
	int	j;

	j = -1;
	*op = 0;
	while (g_vars->iter_else_count > 0 && ++j < g_vars->iter_else_count)
	{
		if (waitpid(-1, &n[stat], 0) != -1)
			(g_vars->ex_status) = n[stat] >> 8;
	}
}

int	nested_par_main_help(int n[SUB], int check, char **arr)
{
	if (check)
	{
		n[skip_op] = not_subshell_skip(n, arr, check);
		if (n[skip_op] != -2)
			return (n[skip_op]);
		if (!arr[n[i]])
			return (0);
	}
	return (-2);
}

int	nested_par_main(int n[SUB], int check, char **arr)
{
	int	res;

	res = nested_par_main_help(n, check, arr);
	if (res != -2)
		return (res);
	if (!g_vars->op)
		g_vars->op = ft_strdup("");
	if (arr[n[i] + 1] || (!arr[n[i] + 1] && (arr[n[i]][0] == '&'
			|| arr[n[i]][0] == '|') && n[ind] > 0))
		g_vars->initial_commands = initial_split(arr[n[i]], 1);
	else
		g_vars->initial_commands = initial_split(arr[n[i]], 0);
	g_vars->iter_else_count++;
	if (!g_vars->initial_commands)
		return (0);
	if (check)
	{
		remove_spaces_in_between();
		if (!parsing_commands(g_vars->initial_commands))
			return (0);
		execution_phase();
		g_vars->command_head = NULL;
	}
	return (-2);
}

int	nested_par_sub(int n[SUB], int check, char **arr)
{
	char	*save;
	char	**kobi;

	n[par_count] = par_coount(arr[n[i]]);
	if (!check && n[par_count] == -1)
	{
		ft_dprintf(2, "minishell: syntax error\n");
		g_vars->ex_status = 2;
		return (0);
	}
	save = ft_strdup (arr[n[i]]);
	n[subshell_op_ret] = subshell_op(n, check, arr);
	if (n[subshell_op_ret] != -2)
		return (n[subshell_op_ret]);
	kobi = split_par(arr[n[i]]);
	n[subshell_err] = subshell_errors(kobi, check);
	if (n[subshell_err] != -2)
		return (n[subshell_err]);
	if (check && (n[ind] > 0 || (n[ind] == 0 && save[0] == '('
				&& save[ft_strlen(save) - 1] == ')')))
		sub_pipe(n, arr, kobi, check);
	else
		return (nested_par(kobi, check, n[ind] + 1));
	return (-2);
}

int	nested_par(char **arr, int check, int index)
{
	int	n[SUB];
	int	res;

	n[i] = -1;
	n[ind] = index;
	while (arr[++n[i]])
	{
		arr[n[i]] = ft_strtrim(arr[n[i]], " \t");
		if (has_char(arr[n[i]], '(') || has_char(arr[n[i]], ')'))
		{
			res = nested_par_sub(n, check, arr);
			if (res != -2)
				return (res);
		}
		else
		{
			if (!subshell_errors2_help(arr, 0))
				return (0);
			not_subshell_pieps(n, arr, check);
			res = nested_par_main(n, check, arr);
			if (res != -2)
				return (res);
		}
	}
	return (1);
}
