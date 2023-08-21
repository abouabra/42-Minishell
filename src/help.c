/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 19:52:24 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 19:10:28 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	checker(char **commands, int i)
{
	int		j;
	char	*str;

	j = i;
	if (!ft_strncmp(commands[i], ">", -1))
		str = ft_strdup(">>");
	if (!ft_strncmp(commands[i], ">>", -1))
		str = ft_strdup(">");
	if (!ft_strncmp(commands[i], "<", -1))
		str = ft_strdup("<<");
	if (!ft_strncmp(commands[i], "<<", -1))
		str = ft_strdup("<");
	while (commands[++j])
	{
		if (!ft_strncmp(commands[j], commands[i], -1)
			|| !ft_strncmp(commands[j], str, -1))
			return (0);
	}
	return (1);
}

int	rederiction_error2(char **commands, int i)
{
	if (commands[i + 1] && (!ft_strncmp(commands[i], ">", -1)
			|| !ft_strncmp(commands[i], "<", -1) || !ft_strncmp(commands[i],
				">>", -1) || !ft_strncmp(commands[i], "<<", -1)) && (
			!ft_strncmp(commands[i + 1], ">", -1)
			|| !ft_strncmp(commands[i + 1], "<", -1)
			|| !ft_strncmp(commands[i + 1], ">>", -1)
			|| !ft_strncmp(commands[i + 1], "<<", -1)))
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
			commands[i + 1]);
		g_vars->ex_status = 2;
		return (0);
	}
	return (1);
}

int	rederiction_error(char **commands, int i)
{
	if (ft_strnstr(commands[i], "<<<", -1) || ft_strnstr(commands[i], ">>>", -1)
		|| ft_strnstr(commands[i], "=>", -1) || (!ft_strncmp(commands[i], ">",
				-1) && commands[i + 1] && !ft_strncmp(commands[i + 1], "<", -1))
		|| (!ft_strncmp(commands[i], "<", -1) && commands[i + 1] && !ft_strncmp(
				commands[i + 1], ">", -1)))
	{
		ft_dprintf(2, "minishell: syntax error\n");
		g_vars->ex_status = 2;
		return (0);
	}
	if ((!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i],
				"<", -1) || !ft_strncmp(commands[i], ">>", -1)
			|| !ft_strncmp(commands[i], "<<", -1)) && !commands[i + 1])
	{
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		g_vars->ex_status = 2;
		return (0);
	}
	if (!rederiction_error2(commands, i))
		return (0);
	return (1);
}

int	par_coount(char *s)
{
	int	i;
	int	active;
	int	par_c;

	i = 0;
	active = 0;
	par_c = 0;
	while (s[i])
	{
		if (s[i] == '(')
			active++;
		if (s[i] == ')')
		{
			active--;
			if (!active)
				par_c++;
		}
		i++;
	}
	if (active)
		return (-1);
	return (par_c);
}

int	skip_opertor(char **arr, int n[SUB], int check, char *s)
{
	while (arr[n[i]] && ((par_coount(arr[n[i]]) == 1 && arr[n[i]][0] == '('
			&& arr[n[i]][ft_strlen(arr[n[i]]) - 1] == ')')
			|| !ft_strnstr(arr[n[i]], s, -1)))
	{
		if (arr[++n[i]])
			arr[n[i]] = ft_strtrim(arr[n[i]], " \t");
	}
	if (arr[n[i]])
	{
		if (ft_strlen(arr[n[i]]) == 2)
			n[i]++;
		if (!par_coount(arr[n[i]]) && arr[n[i]][0] != '('
			&& arr[n[i]][ft_strlen(arr[n[i]]) - 1] != ')')
			return (nested_par(&arr[n[i]], check, n[ind] + 1));
	}
	return (-2);
}
