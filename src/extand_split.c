/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 16:23:08 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/19 16:51:59 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	does_redirection_exist(char *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
		{
			count++;
			i++;
		}
		else if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		{
			count++;
			i++;
		}
		else if ((str[i] == '<' && !str[i + 1]) || (str[i] == '<' && str[i + 1]
				&& str[i + 1] != '<'))
			count++;
		else if ((str[i] == '>' && !str[i + 1]) || (str[i] == '>' && str[i + 1]
				&& str[i + 1] != '>'))
			count++;
	}
	return (count);
}

void	extended_splitting_init(int n[4], char **phrases, char ***arr)
{
	int	count;

	n[i] = -1;
	count = 0;
	while (phrases[++n[i]])
	{
		if (phrases[n[i]][0] != '"' && phrases[n[i]][0] != '\''
			&& !ft_strchr(phrases[n[i]], '='))
		{
			count += split_charset_word_count(phrases[n[i]], "><");
			count += does_redirection_exist(phrases[n[i]]);
		}
		if (ft_strchr(phrases[n[i]], '=') || phrases[n[i]][0] == '"'
			|| phrases[n[i]][0] == '\'')
			count++;
	}
	*arr = my_alloc(sizeof(char *) * (count + 1));
	n[i] = 0;
	n[j] = 0;
	n[k] = 0;
}

void	extended_splitting_help2(char **phrases, int n[4], char **arr)
{
	if ((phrases[n[i]][n[k]] == '<' && phrases[n[i]][n[k] + 1]
			&& phrases[n[i]][n[k] + 1] == '<')
		|| (phrases[n[i]][n[k]] == '>' && phrases[n[i]][n[k] + 1]
			&& phrases[n[i]][n[k] + 1] == '>'))
	{
		arr[n[j]] = ft_substr(phrases[n[i]], n[k], 2);
		n[k]++;
		n[j]++;
	}
	else if ((phrases[n[i]][n[k]] == '<' && !phrases[n[i]][n[k] + 1])
		|| (phrases[n[i]][n[k]] == '<' && phrases[n[i]][n[k] + 1]
			&& phrases[n[i]][n[k] + 1] != '<'))
	{
		arr[n[j]] = ft_substr(phrases[n[i]], n[k], 1);
		n[j]++;
	}
	else if ((phrases[n[i]][n[k]] == '>' && !phrases[n[i]][n[k] + 1])
		|| (phrases[n[i]][n[k]] == '>' && phrases[n[i]][n[k] + 1]
			&& phrases[n[i]][n[k] + 1] != '>'))
	{
		arr[n[j]] = ft_substr(phrases[n[i]], n[k], 1);
		n[j]++;
	}
}

void	extended_splitting_help(char **phrases, int n[4], char **arr)
{
	n[k] = 0;
	while (phrases[n[i]][n[k]])
	{
		if (does_redirection_exist(&phrases[n[i]][n[k]]))
		{
			n[3] = n[k];
			while (phrases[n[i]][n[3]] && phrases[n[i]][n[3]] != '<'
				&& phrases[n[i]][n[3]] != '>')
				n[3]++;
			if (phrases[n[i]][n[3]] && n[3] - n[k] > 0)
			{
				arr[n[j]++] = ft_substr(phrases[n[i]], n[k], n[3] - n[k]);
				n[k] += n[3] - n[k];
			}
			extended_splitting_help2(phrases, n, arr);
		}
		else
		{
			arr[n[j]++] = ft_substr(phrases[n[i]], n[k],
					ft_strlen(phrases[n[i]]) - n[k]);
			n[k] += ft_strlen(phrases[n[i]]) - n[k];
		}
		n[k]++;
	}
}

char	**extended_splitting(char **phrases)
{
	int		n[4];
	char	**arr;

	extended_splitting_init(n, phrases, &arr);
	while (phrases[n[i]])
	{
		if (phrases[n[i]][0] == '"' || phrases[n[i]][0] == '\''
			|| ft_strchr(phrases[n[i]], '='))
		{
			arr[n[j]++] = phrases[n[i]];
			n[i]++;
		}
		if (!phrases[n[i]])
			break ;
		if (!does_redirection_exist(phrases[n[i]]))
		{
			arr[n[j]] = phrases[n[i]];
			n[j]++;
		}
		else
			extended_splitting_help(phrases, n, arr);
		n[i]++;
	}
	arr[n[j]] = NULL;
	return (arr);
}
