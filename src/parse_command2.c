/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/21 15:28:24 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	rm_qts_help(int *num, char **arr, char *q, t_fill_info *info)
{
	char	*new;
	char	**tmp;

	*num = ft_strchr_num(arr[0], q[0]);
	if (arr[0][*num - 1] == '=')
	{
		tmp = ft_split(arr[0], '=');
		new = ft_strtrim(tmp[1], q);
		arr[0] = ft_strjoin(tmp[0], "=");
		arr[0] = ft_strjoin(arr[0], new);
	}
	else
	{
		if (q[0] == '\'')
			info->quote_type = 1;
		else
			info->quote_type = 2;
	}
}

void	fix_string2(t_fill_info *info, char **dest, char **src)
{
	char	c;

	c = *(*src);
	if (c == '\'')
		info->quote_type = 1;
	else if (c == '\"')
		info->quote_type = 2;
	(*src)++;
	while (*(*src) && *(*src) != c)
	{
		*(*dest) = *(*src);
		(*dest)++;
		(*src)++;
	}
	if (*(*src) == c)
		(*src)++;
}

void	fix_string(t_fill_info *info, char *dest, char *src)
{
	if (src[0] && src[1] && !src[2] && src[0] == src[1])
		return ;
	if (ft_strchr(src, '=') || ft_strchr(src, '$'))
		return ;
	while (*src)
	{
		if (*src == '"' || *src == '\'')
			fix_string2(info, &dest, &src);
		else
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	*dest = '\0';
}

void	remove_quotes_help(int quote[2], char **arr)
{
	int	j;
	int	i;

	quote[sin] = 0;
	quote[doub] = 0;
	i = -1;
	while (arr[++i])
	{
		j = -1;
		while (arr[i][++j])
		{
			if (arr[i][j] == '\'')
				quote[sin]++;
			else if (arr[i][j] == '\"')
				quote[doub]++;
		}
	}
}

int	remove_quotes(t_fill_info *info, char **arr)
{
	int	i;
	int	quote[2];

	remove_quotes_help(quote, arr);
	if (quote[sin] % 2 != 0 || quote[doub] % 2 != 0)
	{
		ft_dprintf(2, "minishell: unexpected EOF while looking for matching\n");
		g_vars->ex_status = 2;
		return (0);
	}
	i = -1;
	while (arr[++i])
		fix_string(info, arr[i], arr[i]);
	i = -1;
	while (arr[++i])
		set_quotes_types(info, arr[i]);
	return (1);
}
