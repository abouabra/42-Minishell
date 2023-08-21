/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 10:11:21 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 18:55:59 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strrevcmp(char *s1, char *s2)
{
	int	i;
	int	j;

	j = 0;
	i = ft_strlen(s1) - ft_strlen(s2);
	if (i < 0)
		return (1);
	while (s1[i] && s2[j] && s1[i] == s2[j])
	{
		i++;
		j++;
	}
	return (s1[i] - s2[j]);
}

void	split_char_init(int n[4])
{
	n[i] = -1;
	n[j] = -1;
	n[ac] = 0;
}

void	operations_help(int *i, char *s, char **op, int quote[2])
{
	if ((*i) + 1 < ft_strlen(s) && s[(*i)] == '|' && s[(*i) + 1] == '|'
		&& !quote[doub] && !quote[sin])
	{
		(*op) = ft_strjoin((*op), "2|");
		(*i)++;
	}
	if ((*i) + 1 < ft_strlen(s) && s[(*i)] == '&' && s[(*i) + 1] == '&'
		&& !quote[doub] && !quote[sin])
	{
		(*op) = ft_strjoin((*op), "2&");
		(*i)++;
	}
}

char	*operations(char *s)
{
	char	*op;
	int		i;
	int		quote[2];

	op = "";
	i = -1;
	quote[sin] = 0;
	quote[doub] = 0;
	while (s[++i])
	{
		if (s[i] == '\'' && !quote[doub])
			quote[sin] = !quote[sin];
		if (s[i] == '\"' && !quote[sin])
			quote[doub] = !quote[doub];
		if (((i + 1 < ft_strlen(s) && s[i] == '|' && s[i + 1] != '|') || (i
					+ 1 == ft_strlen(s) && s[i] == '|' && !s[i + 1]))
			&& !quote[doub] && !quote[sin])
			op = ft_strjoin(op, "1|");
		operations_help(&i, s, &op, quote);
	}
	return (op);
}

void	initial_split_init(int n[6])
{
	n[i] = -1;
	n[j] = 0;
	n[k] = 0;
	n[si] = 0;
	n[dou] = 0;
	n[ph_len] = 0;
}
