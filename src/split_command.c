/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:31:17 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/19 16:22:53 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_words(char *s)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;
	int	phrase_count;

	in_single_quote = 0;
	in_double_quote = 0;
	phrase_count = 0;
	i = -1;
	while (++i < ft_strlen(s))
	{
		if (s[i] == ' ' && !in_single_quote && !in_double_quote)
			phrase_count++;
		if (s[i] == '\'')
			if (!in_double_quote)
				in_single_quote = !in_single_quote;
		if (s[i] == '"')
			if (!in_single_quote)
				in_double_quote = !in_double_quote;
	}
	phrase_count++;
	return (phrase_count);
}

static void	initial_vals(int n[3], int in_quotes[3])
{
	in_quotes[sin] = 0;
	in_quotes[doub] = 0;
	n[i] = 0;
	n[j] = 0;
	n[k] = 0;
	in_quotes[in_word] = 0;
}

static void	quotes_in(int in_quotes[3], int n[3], char *s)
{
	in_quotes[in_word] = 1;
	if (s[n[i]] == '\'')
		if (!in_quotes[doub])
			in_quotes[sin] = !in_quotes[sin];
	if (s[n[i]] == '"')
		if (!in_quotes[sin])
			in_quotes[doub] = !in_quotes[doub];
}

static void	handle_cases(char *s, int n[3], int in_quotes[3], char **phrases)
{
	if (s[n[i]] == ' ' && !in_quotes[sin] && !in_quotes[doub])
	{
		if (in_quotes[in_word])
		{
			n[ph_len] = n[i] - n[j];
			phrases[n[k]] = (char *)my_alloc((n[ph_len] + 1) * sizeof(char));
			ft_memcpy(phrases[n[k]], s + n[j], n[ph_len]);
			phrases[n[k]][n[ph_len]] = '\0';
			n[k]++;
			n[j] = n[i] + 1;
			in_quotes[in_word] = 0;
		}
		else
			n[j]++;
	}
	else
		quotes_in(in_quotes, n, s);
	n[i]++;
}

char	**split_command(char *s)
{
	char	**phrases;
	int		n[4];
	int		in_quotes[3];
	int		phrase_count;

	n[ph_len] = 0;
	initial_vals(n, in_quotes);
	phrase_count = count_words(s);
	phrases = (char **)my_alloc((phrase_count + 1) * sizeof(char *));
	while (n[i] < ft_strlen(s))
		handle_cases(s, n, in_quotes, phrases);
	if (in_quotes[in_word])
	{
		n[ph_len] = n[i] - n[j];
		phrases[n[k]] = (char *)my_alloc((n[ph_len] + 1) * sizeof(char));
		ft_memcpy(phrases[n[k]], s + n[j], n[ph_len]);
		phrases[n[k]][n[ph_len]] = '\0';
		n[k]++;
	}
	phrases[n[k]] = NULL;
	phrases = extended_splitting(phrases);
	return (phrases);
}
