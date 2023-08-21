/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:15:02 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/21 15:07:28 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	dollar_active_help(int n[4], char *s[4])
{
	while (s[str][++n[k]] && s[str][n[k]] != '\''
		&& s[str][n[k]] != '"' && s[str][n[k]] != ' '
		&& s[str][n[k]] != '-')
	{
		if (s[str][n[k]] == '?')
		{
			n[k]++;
			break ;
		}
		if (s[str][n[k]] == '$')
			break ;
		if (s[str][n[k]] == '@')
		{
			n[k]++;
			break ;
		}
		if (ft_isdigit(s[str][n[k]]))
		{
			n[k]++;
			break ;
		}
	}
	if (!s[str][n[k]])
		n[k]++;
}

void	dollar_active3(int n[4], char *s[4], char **args)
{
	if (!ft_strncmp(s[str] + 1, "?", -1))
		args[n[i]] = ft_strjoin(s[news], ft_itoa((g_vars->ex_status)));
	else
	{
		if (s[data])
		{
			args[n[i]] = ft_strjoin(s[news], s[data]);
			args[n[i]] = ft_strjoin(args[n[i]],
					ft_substr(s[str], n[k], -1));
		}
		else
			args[n[i]] = ft_strjoin(s[news], "");
	}
	s[str] += n[k];
	n[k] = 0;
}

void	dollar_active2(int n[4], char *s[4], char **args)
{
	dollar_active_help(n, s);
	s[tmp] = ft_substr(s[str], 1, n[k] - 1);
	if (!s[tmp][0])
		s[data] = "$";
	else if (s[tmp][0] == '?')
		s[data] = ft_itoa(g_vars->ex_status);
	else if (s[tmp][0] == '@')
		s[data] = "";
	else if (ft_isdigit(s[tmp][0]))
	{
		s[data] = args[ft_atoi(s[tmp])];
		s[data] = "";
	}
	else
		s[data] = get_env_data(s[tmp]);
	n[j] = ft_strchr_num(args[n[i]], '$');
	s[news] = ft_substr(args[n[i]], 0, n[j]);
}

void	dollar_active(t_fill_info *info, int n[4], char *s[4], char **args)
{
	int	sould_remove_space;

	sould_remove_space = 0;
	if (info->quote_type == 0)
		sould_remove_space = 1;
	n[k] = 0;
	while (s[str][n[k]])
	{
		if (!ft_strchr(s[str], '$'))
			return ;
		dollar_active2(n, s, args);
		dollar_active3(n, s, args);
	}
}
