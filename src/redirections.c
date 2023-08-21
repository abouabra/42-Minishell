/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 20:55:49 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/19 21:03:47 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_a_redirection(char *str)
{
	if (!str)
		return (0);
	if (!ft_strncmp(str, ">", -1) || !ft_strncmp(str, "<", -1)
		|| !ft_strncmp(str, ">>", -1) || !ft_strncmp(str, "<<", -1))
		return (1);
	return (0);
}

void	red_help2(t_fill_info *info, char **commands, int *i, char *file_name)
{
	char		*heredoc_file;
	t_cmd_redir	*redir;

	if (!ft_strncmp(commands[*i], "<", -1))
	{
		redir = ft_new_redir(INPUT, file_name);
		(*i)++;
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], ">>", -1))
	{
		redir = ft_new_redir(APPEND, file_name);
		(*i)++;
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<<", -1))
	{
		heredoc_file = get_herdoc_data(info, commands[++(*i)]);
		redir = ft_new_redir(HEREDOC, heredoc_file);
		add_redir_in_back(&info->redir, redir);
	}
}

void	red_help(t_fill_info *info, char **commands, int *i)
{
	char		**arr;
	char		*file_name;
	int			k;
	t_cmd_redir	*redir;

	if (is_a_redirection(commands[*i]) && ft_strchr(commands[*i + 1], '$'))
	{
		arr = my_alloc(sizeof(char *) * 2);
		arr[0] = ft_strdup(commands[*i + 1]);
		arr[1] = NULL;
		arr = expand_variables(info, arr);
		k = -1;
		while (arr[++k])
			fix_string(info, arr[k], arr[k]);
		file_name = ft_strtrim(*arr, " ");
	}
	else
		file_name = commands[*i + 1];
	if (!ft_strncmp(commands[*i], ">", -1))
	{
		redir = ft_new_redir(OUTPUT, file_name);
		(*i)++;
		add_redir_in_back(&info->redir, redir);
	}
	red_help2(info, commands, i, file_name);
}

void	count_redirections_help(char *command, int *i, int *count)
{
	if (command[(*i)] == '<')
	{
		if ((*i) + 1 < len && command[(*i) + 1] != ' ' && command[(*i)
				+ 1] == '<')
		{
			(*i)++;
			(*count)++;
		}
		else if ((*i) + 1 < len && command[(*i) + 1] != ' ' && command[(*i)
				+ 1] != '<')
			(*count)++;
	}
	else if (command[(*i)] == '>')
	{
		if (((*i) + 1 < len) && command[(*i) + 1] != ' ' && command[(*i)
				+ 1] != '>')
		{
			(*i)++;
			(*count)++;
		}
		else if (command[(*i) + 1] != ' ' && command[(*i) + 1] == '>')
			(*count)++;
	}
}

int	count_redirections(char *command)
{
	int	count;
	int	len;
	int	i;
	int	is_parsing_command;

	count = 0;
	len = ft_strlen(command);
	i = 0;
	is_parsing_command = 1;
	while (i < len)
	{
		if (command[i] == '<' || command[i] == '>')
		{
			count_redirections_help(command, &i, &count);
			is_parsing_command = 0;
		}
		else if (command[i] == ' ' && !is_parsing_command)
		{
			is_parsing_command = 1;
		}
		i++;
	}
	if (is_parsing_command && (len > 0 && command[len - 1] != ' '))
		count++;
	return (count);
}
