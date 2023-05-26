/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 19:52:24 by ykhayri           #+#    #+#             */
/*   Updated: 2023/05/26 22:51:57 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	checker(char **commands, int i)
{
	int		j;
	char	*str;

	j = i;
	if (!ft_strncmp(commands[i], ">", -1))
		str = ft_strdup(">>");
	if (!ft_strncmp(commands[i], ">>", -1))
		str = ft_strdup(">");
	if (!ft_strncmp(commands[i], "<", -1))
		str = ft_strdup("<<");
	if (!ft_strncmp(commands[i], "<<", -1))
		str = ft_strdup("<");
	while (commands[++j])
	{
		if (!ft_strncmp(commands[j], commands[i], -1)
			|| !ft_strncmp(commands[j], str, -1))
			return (0);
	}
	return (1);
}

void	rederiction_error(char **commands, int i)
{
	if ((!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i],
				"<", -1) || !ft_strncmp(commands[i], ">>", -1)
			|| !ft_strncmp(commands[i], "<<", -1)) && !commands[i + 1])
	{
		ft_dprintf(1,
			"minishell: syntax error near unexpected token `newline'\n");
		custom_exit(2);
	}
	if (commands[i + 1] && (!ft_strncmp(commands[i], ">", -1)
			|| !ft_strncmp(commands[i], "<", -1) || !ft_strncmp(commands[i],
				">>", -1) || !ft_strncmp(commands[i], "<<", -1)) && (
			!ft_strncmp(commands[i + 1], ">", -1)
			|| !ft_strncmp(commands[i + 1], "<", -1)
			|| !ft_strncmp(commands[i + 1], ">>", -1)
			|| !ft_strncmp(commands[i + 1], "<<", -1)))
	{
		ft_dprintf(1, "minishell: syntax error near unexpected token `%s'\n",
			commands[i + 1]);
		custom_exit(2);
	}
}

void	dollar_active(int n[4], char *strings[4], char **args)
{
	n[k] = -1;
	while (strings[str][++n[k]] && strings[str][n[k]] != '\''
		&& strings[str][n[k]] != '"' && strings[str][n[k]] != ' ')
		;
	if (!strings[str][n[k]])
		n[k]++;
	strings[tmp] = ft_substr(strings[str], 1, n[k] - 1);
	strings[data] = get_env_data( strings[tmp]);
	n[j] = ft_strchr_num(args[n[i]], '$');
	strings[news] = ft_substr(args[n[i]], 0, n[j]);
	if (!ft_strncmp(strings[str] + 1, "?", -1))
		args[n[i]] = ft_strjoin(strings[news], ft_itoa(*(vars->ex_status)));
	else
	{
		if (strings[data])
		{
			args[n[i]] = ft_strjoin(strings[news], strings[data]);
			args[n[i]] = ft_strjoin(args[n[i]],
					ft_substr(strings[str], n[k], -1));
		}
		else
			args[n[i]] = ft_strjoin(strings[news], "");
	}
}

void	nested_par(char **arr)
{
	char	**tmp;
	int		i;
	int		j;

	i = -1;
	while (arr[++i])
	{
		if (has_char(arr[i], '('))
			nested_par(split_par(arr[i]));
		else
		{
			j = -1;
			tmp = initial_split( arr[i], 1);
			if (!tmp)
				return ;
			while (tmp[++j])
				tmp[j] = ft_strtrim(tmp[j], " ");
			parsing_commands( tmp);
			vars->command_head = NULL;
		}
	}
}

void	split_char_init(int n[4])
{
	n[i] = -1;
	n[j] = -1;
	n[ac] = 0;
}
