/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 19:52:24 by ykhayri           #+#    #+#             */
/*   Updated: 2023/06/01 21:09:46 by abouabra         ###   ########.fr       */
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

int rederiction_error(char **commands, int i)
{
	if (ft_strnstr(commands[i], "<<<", -1) || ft_strnstr(commands[i], ">>>", -1)
	|| ft_strnstr(commands[i], "=>", -1)
	|| (!ft_strncmp(commands[i], ">", -1) && commands[i + 1] && !ft_strncmp(commands[i + 1], "<", -1))
	|| (!ft_strncmp(commands[i], "<", -1) && commands[i + 1] && !ft_strncmp(commands[i + 1], ">", -1)))
	{
		ft_dprintf(2, "minishell: syntax error\n");
		*vars->ex_status = 2;
		return 0;
	}
	if ((!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i],
				"<", -1) || !ft_strncmp(commands[i], ">>", -1)
			|| !ft_strncmp(commands[i], "<<", -1)) && !commands[i + 1])
	{
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		*vars->ex_status = 2;
		return(0);
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
		*vars->ex_status = 2;
		return 0;
	}
	return 1;
}

void	dollar_active(t_fill_info *info, int n[4], char *strings[4], char **args)
{
	// ft_dprintf(1, "Start: |%s|   |%d|\n", strings[str],info->quote_type);
	int sould_remove_space = 0;
	if(info->quote_type == 0)
		sould_remove_space = 1;
	
	n[k] = 0;
	if(strings[str][n[k] -1] && strings[str][n[k] -1] == '\'')
	{
		info->quote_type = 1;
		return;	
	}
	while(strings[str][n[k]])
	{		
		if(!ft_strchr(strings[str], '$'))
			return;
		// ft_dprintf(1, "loop: |%s|\n",strings[str]);
		while (strings[str][++n[k]] && strings[str][n[k]] != '\''
			&& strings[str][n[k]] != '"' && strings[str][n[k]] != ' '
			&& strings[str][n[k]] != '-')
		{
			if(strings[str][n[k]] == '?')
			{
				n[k]++;
				break;
			}
			if(strings[str][n[k]] == '$')
				break;
			if(strings[str][n[k]] == '@')
			{
				n[k]++;
				break;
			}
			if(ft_isdigit(strings[str][n[k]]))
			{
				n[k]++;
				break;
			}
		}

		if (!strings[str][n[k]])
			n[k]++;
		strings[tmp] = ft_substr(strings[str], 1, n[k] - 1);

		if(!strings[tmp][0])
			strings[data] = "$";
		else if(strings[tmp][0] == '?')
			strings[data] = ft_itoa(*vars->ex_status);
		else if(strings[tmp][0] == '@')
			strings[data] = "";
		else if(ft_isdigit(strings[tmp][0]))
		{
			strings[data] = args[ft_atoi(strings[tmp])];
			strings[data] = "";
		}
		else
			strings[data] = get_env_data( strings[tmp]);
		


		n[j] = ft_strchr_num(args[n[i]], '$');
		strings[news] = ft_substr(args[n[i]], 0, n[j]);
		// if(sould_remove_space)
			// strings[data] = ft_strtrim(strings[data], " ");
		

		// ft_dprintf(1, "substr: |%s|   %d   data: |%s|\n\n\n\n\n",strings[tmp],sould_remove_space,strings[data]);

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
		
		strings[str] += n[k];
		n[k] = 0;
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
