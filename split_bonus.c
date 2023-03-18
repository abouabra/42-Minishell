/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:37:37 by ykhayri           #+#    #+#             */
/*   Updated: 2023/03/18 16:16:18 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	n[i] = -1;
	n[j] = -1;
	n[ac] = 0;
	if (!s || !s[0])
		return (0);
	n[w] = count_par(s);
	arr = (char **) my_alloc(sizeof(char *) * n[w] + 1);
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

void	nested_par(char **arr, t_args *vars)
{
	char	**tmp;
	int		i;
	int		j;

	i = -1;
	while (arr[++i])
	{
		if (has_char(arr[i], '('))
			nested_par(split_par(arr[i]), vars);
		else
		{
			j = -1;
			tmp = initial_split(vars, arr[i], 1);
			if (!tmp)
				return ;
			while (tmp[++j])
				// printf("|%s|\n", ft_strtrim(tmp[j]," "));
				tmp[j] = ft_strtrim(tmp[j], " ");
			parsing_commands(vars, tmp);
			printf("-------------------------------\n");
			debug_menu(vars);
			printf("-------------------------------\n");
			vars->command_head = NULL;
		}
	}
}
/*
int main(int ac, char **av)
{
	char **arr;
	t_args vars;
	arr = split_par(av[1]);
	nested_par(arr, &vars);
	return (0);
}*/
