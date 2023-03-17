/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/17 22:22:41 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_dprintf.h"
#include "libft/get_next_line.h"
#include "minishell.h"

static void	rm_qts_help(int *num, char **arr, char *q, t_fill_info *info)
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
		arr[0] = ft_strtrim(arr[0], q);
		if (q[0] == '\'')
			info->quote_type = 1;
		else
			info->quote_type = 2;
	}
}

void	remove_quotes(t_args *vars, t_fill_info *info, char **arr)
{
	int		i;
	int		num_1;
	int		num_2;

	(void) vars;
	i = -1;
	while (arr[++i])
	{
		if (arr[i][0] == '\'')
			rm_qts_help(&num_1, &arr[i], "\'", info);
		else if (arr[i][0] == '\"')
			rm_qts_help(&num_2, &arr[i], "\"", info);
	}
}

char	*get_herdoc_data(char *limiter)
{
	char	*str;
	char	*total;

	limiter = ft_strjoin(limiter, "\n");
	total = "";
	while (1)
	{
		ft_dprintf(1, "> ");
		str = get_next_line(0);
		if (!str || !ft_strncmp(str, limiter, -1))
			break ;
		total = ft_strjoin(total, str);
	}
	return (total);
}

static void	red_help(t_fill_info *info, char **commands, int *i)
{
	if (!ft_strncmp(commands[*i], ">", -1))
	{
		check_permision(NULL, commands[*i + 1], 3);
		info->is_output = 1;
		info->output_file = commands[++(*i)];
	}
	if (!ft_strncmp(commands[*i], "<", -1))
	{
		check_permision(NULL, commands[*i + 1], 2);
		info->is_input = 1;
		info->input_file = commands[++(*i)];
	}
	if (!ft_strncmp(commands[*i], ">>", -1))
	{
		check_permision(NULL, commands[*i + 1], 3);
		info->is_output = 1;
		info->is_append = 1;
		info->append_file = commands[++(*i)];
	}
	if (!ft_strncmp(commands[*i], "<<", -1))
	{
		info->is_herdoc = 1;
		info->herdoc_limiter = commands[++(*i)];
		info->herdoc_data = get_herdoc_data(info->herdoc_limiter);
	}
}

void	parse_redirections(t_fill_info *info, char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
	{
		if ((!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i], "<", -1)
			|| !ft_strncmp(commands[i], ">>", -1) || !ft_strncmp(commands[i], "<<", -1))
				&& !commands[i + 1])
		{
			ft_dprintf(1, "minishell: syntax error near unexpected token `newline'\n");
			custom_exit(2);
		}
		if (commands[i + 1] && (!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i], "<", -1)
			|| !ft_strncmp(commands[i], ">>", -1) || !ft_strncmp(commands[i], "<<", -1))
			&& (!ft_strncmp(commands[i + 1], ">", -1) || !ft_strncmp(commands[i + 1], "<", -1)
			|| !ft_strncmp(commands[i + 1], ">>", -1) || !ft_strncmp(commands[i + 1], "<<", -1)))
		{
			ft_dprintf(1, "minishell: syntax error near unexpected token `%s'\n", commands[i + 1]);
			custom_exit(2);
		}
		red_help(info, commands, &i);
		i++;
	}
}
