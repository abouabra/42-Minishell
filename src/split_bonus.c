/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:37:37 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/20 18:56:06 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_par(char *s)
{
	int	n[3];

	n[i] = -1;
	n[w] = 0;
	n[j] = 0;
	while (s[++n[i]])
	{
		if (s[n[i]] == '(')
			n[w]++;
		else if (s[n[i]] == ')')
		{
			n[w]--;
			if (!n[w])
				n[j]++;
		}
		else if (!n[w] && s[n[i]] > 32)
		{
			while (s[n[i]] && s[n[i]] > 32)
				n[i]++;
			n[j]++;
			if (!s[n[i]])
				return (n[j]);
		}
	}
	return (n[j]);
}

static int	word_len(char *s, char *c)
{
	int	i;
	int	active;

	i = 0;
	active = 1;
	if (c[0] == ' ')
	{
		while (s[i] && s[i] != c[1])
			i++;
	}
	else
	{
		while (active && s[i])
		{
			if (s[i] == '(')
				active++;
			else if (s[i] == ')')
				active--;
			if (!active)
				return (i);
			i++;
		}
	}
	return (i);
}

static void	split_par_help(char **arr, char *s, int n[4])
{
	n[ac]++;
	arr[++n[j]] = my_alloc(word_len(s + n[i], ")") + 1);
	while (n[ac] > 0)
	{
		if (s[++n[i]] == '(')
			n[ac]++;
		else if (s[n[i]] == ')')
		{
			n[ac]--;
			if (!n[ac])
				break ;
		}
		if (!s[n[i]] && n[ac] != 0)
		{
			arr[n[j]] = 0;
			return ;
		}
		arr[n[j]][++n[w]] = s[n[i]];
	}
	arr[n[j]][++n[w]] = '\0';
	arr[n[j]] = ft_strjoin("(", arr[n[j]]);
	arr[n[j]] = ft_strjoin(arr[n[j]], ")");
}

static void	split_par_help2(char **arr, char *s, int n[4])
{
	arr[++n[j]] = my_alloc(word_len(s + n[i], " (") + 1);
	while (s[n[i]] && s[n[i]] != '(')
		arr[n[j]][++n[w]] = s[n[i]++];
	arr[n[j]][++n[w]] = '\0';
	if (s[n[i]] == '(')
		n[i]--;
}

char	**split_par(char *s)
{
	char	**arr;
	int		n[4];

	split_char_init(n);
	if (!s || !s[0])
		return (0);
	n[w] = count_par(s);
	arr = (char **)my_alloc(sizeof(char *) * n[w] + 1);
	if (!arr)
		return (0);
	while (s[++n[i]])
	{
		n[w] = -1;
		if (s[n[i]] == '(')
			split_par_help(arr, s, n);
		else if (!n[ac] && s[n[i]] != ' ')
			split_par_help2(arr, s, n);
		if (!arr[n[j]])
			return (0);
		if (!s[n[i]])
			break ;
	}
	arr[++n[j]] = 0;
	return (arr);
}
