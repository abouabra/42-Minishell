/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambig.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 16:05:53 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/21 15:28:29 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	test_ambiguous_help(t_fill_info *in, char **arg, int i)
{
	char	*name;
	char	**arr;
	int		k;

	name = ft_strdup(arg[i + 1]);
	arr = my_alloc(sizeof(char *) * 2);
	arr[0] = ft_strdup(arg[i + 1]);
	arr[1] = NULL;
	arr = expand_variables(in, arr);
	k = -1;
	while (arr[++k])
		fix_string(in, arr[k], arr[k]);
	arr = split_command(*arr);
	k = -1;
	while (arr[++k])
		;
	if (k != 1)
	{
		ft_dprintf(2, "minishell: %s: ambiguous redirect\n", name);
		g_vars->ex_status = 1;
		return (0);
	}
	if (!rederiction_error(arg, i))
		return (0);
	return (1);
}

int	test_ambiguous(t_fill_info *in, char **arg)
{
	int		i;

	i = -1;
	while (arg[++i])
	{
		if (does_redirection_exist(arg[i]) && arg[i + 1] && ft_strncmp(arg[i],
				"<<", -1))
		{
			if (!test_ambiguous_help(in, arg, i))
				return (0);
		}
		else if (does_redirection_exist(arg[i]) && !arg[i + 1])
			if (!rederiction_error(arg, i))
				return (0);
	}
	return (1);
}

void	subshell_errors2_help_quotes(int quote[2], char c)
{
	if (c == '\'' && !quote[doub])
		quote[sin] = !quote[sin];
	if (c == '\"' && !quote[sin])
		quote[doub] = !quote[doub];
}

void	exp_var_quote(t_fill_info *info, char **args, int n[4], char *strs[4])
{
	int	i;

	if (info->quote_type != 1)
		dollar_active(info, n, strs, args);
	else if (info->quote_type == 1)
	{
		i = -1;
		while (args[++i])
			args[i] = ft_strtrim(args[i], "\'");
	}
}

void	set_quotes_types(t_fill_info *info, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' && (!str[i + 1]
				|| (str[i + 1] && str[i + 1] != '\'')))
			info->quote_type = 1;
		else if (str[i] == '\"' && (!str[i + 1]
				|| (str[i + 1] && str[i + 1] != '\"')))
			info->quote_type = 2;
	}
}
