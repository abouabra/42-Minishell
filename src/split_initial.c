/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_initial.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:31:10 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/03 00:24:52 by ayman            ###   ########.fr       */
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
	int		i;
	int		phrase_count;
	int		q[2];

	i = -1;
	phrase_count = 0;
	q[sin] = 0;
	q[doub] = 0;
	while (++i < ft_strlen(s))
	{
		doub_sin_skip(&q[sin], &q[doub], s, i);
		if (s[i] == '|' && s[i + 1] != '|' && !q[doub] && !q[doub])
			phrase_count++;
		else if (s[i] == '&' && s[i + 1] == '&' && !q[doub] && !q[doub])
		{
			phrase_count++;
			i++;
		}
		else if (s[i] == '|' && s[i + 1] == '|' && !q[doub] && !q[doub])
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

static int	check_validity(char **phrases, int phrase_count)
{
	int	i;

	i = -1;
	while (phrases[++i])
	{
		if (!phrases[i][0] || !ft_strtrim(phrases[i], " \t\n")[0])
		{
			if (phrase_count > 1)
			{
				ft_dprintf(1, "minishell: syntax error\n");
				*vars->ex_status = 2;
				return (0);
			}
			else
				return (0);
		}
	}
	return (1);
}

char *operations(char *s){
	char *op;
	int i;
	
	op="";
	i = -1;
	while (s[++i])
	{
		if (i + 1 < ft_strlen(s) && s[i] == '|' && s[i + 1] != '|')
			op = ft_strjoin(op, "1|");
		if (i + 1 < ft_strlen(s) && s[i] == '|' && s[i + 1] == '|')
		{
			op = ft_strjoin(op, "2|");
			i++;
			}
		if (i + 1 < ft_strlen(s) && s[i] == '&' && s[i + 1] == '&')
		{
			op = ft_strjoin(op, "2&");
			i++;
		}
	}
	return (op);
}

char	**initial_split(char *s, int sw)
{
	int		n[6];
	int		phrase_count;
	int		ph_len;
	char	**phrases;

	n[i] = -1;
	n[j] = 0;
	n[k] = 0;
	n[si] = 0;
	n[dou] = 0;
	ph_len = 0;
	phrase_count = count_words(s);
	vars->command_count = phrase_count;
	phrases = (char **)my_alloc((phrase_count + 1) * sizeof(char *));
	vars->op = operations(s);
	while (++n[i] < ft_strlen(s))
		split_cases(n, ph_len, s, phrases);
	ph_len = n[i] - n[j];
	phrases[n[k]] = (char *)my_alloc((ph_len + 1) * sizeof(char));
	ft_memcpy(phrases[n[k]], s + n[j], ph_len);
	phrases[n[k]][ph_len] = '\0';
	n[k]++;
	phrases[n[k]] = NULL;
	if (!sw && !check_validity(phrases, phrase_count))
		return (NULL);
	return (phrases);
}
