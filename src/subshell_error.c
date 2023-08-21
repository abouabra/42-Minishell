/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 19:02:02 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/21 11:49:31 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	subshell_errors2_help(char **op_err, int y)
{
	int	w;
	int	op_c;
	int	quote[2];

	w = -1;
	op_c = 0;
	quote[sin] = 0;
	quote[doub] = 0;
	while (op_err[y][++w])
	{
		subshell_errors2_help_quotes(quote, op_err[y][w]);
		if ((op_err[y][w] == '|' || op_err[y][w] == '&')
			&& !quote[doub] && !quote[sin])
			op_c++;
		if (op_c > 2)
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
		if (op_err[y][w] != '|' && op_err[y][w] != '&'
			&& !quote[doub] && !quote[sin])
			op_c = 0;
	}
	return (1);
}

int	subshell_errors4(char **kobi, int x)
{
	if (kobi[x + 1])
	{
		kobi[x + 1] = ft_strtrim(kobi[x + 1], " \t");
		if (kobi[x + 1][0] != '&' && kobi[x + 1][0] != '|')
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	if (x > 0)
	{
		kobi[x -1] = ft_strtrim(kobi[x - 1], " \t");
		if (kobi[x -1][ft_strlen(kobi[x - 1]) - 1] != '&'
			&& kobi[x -1][ft_strlen(kobi[x - 1]) - 1] != '|')
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	return (1);
}

int	subshell_errors3(char **kobi, int x)
{
	if (kobi[x][0] == '&' || kobi[x][0] == '|')
	{
		if (!x || (x - 1 >= 0 && par_coount(kobi[x -1]) == 0))
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	if (kobi[x][ft_strlen(kobi[x]) - 1] == '&'
		|| kobi[x][ft_strlen(kobi[x]) - 1] == '|')
	{
		if (!kobi[x + 1] || (kobi[x + 1] && par_coount(kobi[x + 1]) == 0))
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	return (1);
}

int	subshell_errors2(char **kobi, int x)
{
	char	**op_err;
	int		y;

	y = -1;
	op_err = ft_split_charset(kobi[x], " \t");
	while (op_err[++y])
	{
		if (!subshell_errors2_help(op_err, y))
			return (0);
		if (op_err[y + 1] && (op_err[y][ft_strlen(op_err[y]) - 1] == '&'
			|| op_err[y][ft_strlen(op_err[y]) - 1] == '|')
		&& (op_err[y + 1][0] == '&' || op_err[y +1][0] == '|'))
		{
			ft_dprintf(2, "minishell: syntax error\n");
			g_vars->ex_status = 2;
			return (0);
		}
	}
	return (1);
}

int	subshell_errors(char **kobi, int check)
{
	int	x;

	x = -1;
	while (!check && kobi[++x])
	{
		kobi[x] = ft_strtrim(kobi[x], " \t");
		if (!subshell_errors2(kobi, x))
			return (0);
		if (!subshell_errors3(kobi, x))
			return (0);
		if (par_coount(kobi[x]) == 1 && kobi[x][0] == '('
			&& kobi[x][ft_strlen(kobi[x]) - 1] == ')')
		{
			if (!subshell_errors4(kobi, x))
				return (0);
		}
	}
	return (-2);
}
