/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 21:39:20 by ykhayri           #+#    #+#             */
/*   Updated: 2023/03/15 21:39:54 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*wildcard(t_args *vars, char **av)
{
	DIR *dir;
	struct dirent *read;
	int	v;
	char **arr;
	int i;
	int j;
	int is_valid;
	char *tmp;
	char *final;
	int k;
	(void)vars;
	dir = opendir(getcwd(NULL,-1));
	arr = ft_split(av[1], '*');
	is_valid = 0;
	final = "";
	k = 0;
	i = -1;
	while(arr[++i]);
	while(++k)
	{
		read = readdir(dir);
		if(!read)
			break;
		if(k > 2 && i == 0 && read->d_name[0] != '.')
			final = ft_strjoin(final,ft_strjoin(read->d_name," "));
		else if(k > 2 && i == 1)
		{
			v = ft_strlen(arr[0])-1;
			if (v < 1)
				v = 1;
			if(av[1][0] == '*' && av[1][ft_strlen(av[1])-1] != '*' && !ft_strrevcmp(read->d_name,arr[0]))
				final = ft_strjoin(final,ft_strjoin(read->d_name," "));
			else if (av[1][0] != '*' && av[1][ft_strlen(av[1])-1] == '*' && !ft_strncmp(read->d_name, arr[0], v))
				final = ft_strjoin(final,ft_strjoin(read->d_name," "));
			else if (av[1][0] == '*' && av[1][ft_strlen(av[1])-1] == '*' && ft_strnstr(read->d_name, arr[0], -1))
				final = ft_strjoin(final,ft_strjoin(read->d_name," "));
		}
		else if (k > 2 && i > 1)
		{
			j = -1;
			tmp = read->d_name;
			is_valid = 0;
			while (++j < i)
			{
				if(j == i-1 && av[1][ft_strlen(av[1])-1] != '*' && !ft_strrevcmp(read->d_name,arr[j]))
					is_valid++;
				else if(j == 0 && av[1][0] != '*' && !ft_strncmp(read->d_name, arr[j], ft_strlen(arr[j])-1))
					is_valid++;
				else if((j > 0 && j < i -1) || (av[1][ft_strlen(av[1])-1] == '*' && j == i-1) || (av[1][0] == '*' && j == 0))
				{
					tmp = ft_strnstr(tmp, arr[j], -1);
					if(tmp)
						is_valid++;	
				}
			}
			if (is_valid == i)
				final = ft_strjoin(final,ft_strjoin(read->d_name," "));
		}
	}
	closedir(dir);
	return (final);
}
