/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_initial.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:31:10 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 22:35:59 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_separator(int i, int j, char *s)
{
	int	k;
	int	f;
	int	q[2];

	k = 0;
	f = -1;
	q[sin] = 0;
	q[doub] = 0;
	if (s[i] == '|' || (s[i] == '&' && s[j] == '&'))
		k++;
	while (++f < i)
		doub_sin_skip(&q[sin], &q[doub], s, f);
	if (k && !q[doub] && !q[sin])
		return (1);
	return (0);
}

static int	count_words(char *s)
{
	int	i;
	int	phrase_count;
	int	q[2];

	i = -1;
	phrase_count = 0;
	q[sin] = 0;
	q[doub] = 0;
	while (++i < ft_strlen(s))
	{
		doub_sin_skip(&q[sin], &q[doub], s, i);
		if (s[i] == '|' && s[i + 1] != '|' && !q[doub] && !q[sin])
			phrase_count++;
		else if (s[i] == '&' && s[i + 1] == '&' && !q[doub] && !q[sin])
		{
			phrase_count++;
			i++;
		}
		else if (s[i] == '|' && s[i + 1] == '|' && !q[doub] && !q[sin])
		{
			phrase_count++;
			i++;
		}
	}
	return (++phrase_count);
}

static void	split_cases(int n[6], int ph_len, char *s, char **phrases)
{
	doub_sin_skip(&n[si], &n[dou], s, n[i]);
	if (has_separator(n[i], n[i] + 1, s))
	{
		ph_len = n[i] - n[j];
		phrases[n[k]] = (char *)my_alloc((ph_len + 1) * sizeof(char));
		ft_memcpy(phrases[n[k]], s + n[j], ph_len);
		phrases[n[k]][ph_len] = '\0';
	}
	if (s[n[i]] == '|' && s[n[i] + 1] != '|' && !n[si] && !n[dou])
	{
		n[k]++;
		n[j] = n[i] + 1;
	}
	else if (s[n[i]] == '&' && s[n[i] + 1] == '&' && !n[si] && !n[dou])
	{
		n[k]++;
		n[j] = n[i] + 2;
		n[i]++;
	}
	else if (s[n[i]] == '|' && s[n[i] + 1] == '|' && !n[si] && !n[dou])
	{
		n[k]++;
		n[j] = n[i] + 2;
		n[i]++;
	}
}

static int	check_validity(char **phrases, int phrase_count, int sw)
{
	int	i;

	i = -1;
	while (phrases[++i])
	{
		phrases[i] = ft_strtrim(phrases[i], " \t\n");
		if ((!sw && (!phrases[i][0])))
		{
			if ((!sw && phrase_count > 1) || (!sw && !phrases[i][0] && i))
			{
				ft_dprintf(2, "minishell: syntax error\n");
				g_vars->ex_status = 2;
				return (0);
			}
			else
			{
				return (0);
			}
		}
	}
	return (1);
}

char	**initial_split(char *s, int sw)
{
	int		n[6];
	int		phrase_count;
	char	**phrases;

	initial_split_init(n);
	phrase_count = count_words(s);
	g_vars->command_count = phrase_count;
	g_vars->op = operations(s);
	if (sw == 1 && g_vars->op[0])
		g_vars->command_count--;
	if (sw == 1 && ft_strlen(g_vars->op) >= 2 && (s[i] == '|' || s[i] == '&')
		&& (s[ft_strlen(s) - 1] == '|' || s[ft_strlen(s) - 1] == '&'))
		g_vars->command_count--;
	phrases = (char **)my_alloc((phrase_count + 1) * sizeof(char *));
	while (++n[i] < ft_strlen(s))
		split_cases(n, n[ph_len], s, phrases);
	n[ph_len] = n[i] - n[j];
	phrases[n[k]] = (char *)my_alloc((n[ph_len] + 1) * sizeof(char));
	ft_memcpy(phrases[n[k]], s + n[j], n[ph_len]);
	phrases[n[k]][n[ph_len]] = '\0';
	n[k]++;
	phrases[n[k]] = NULL;
	if (!check_validity(phrases, phrase_count, sw))
		return (NULL);
	return (phrases);
}
