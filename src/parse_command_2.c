/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/28 22:35:27 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	remove_quotes(t_fill_info *info, char **arr)
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
	char *name = "/tmp/herdoc_data";
	int fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	write(fd, total, ft_strlen(total));
	close(fd);
	return (name);
}

static void	red_help(t_fill_info *info, char **commands, int *i)
{
	if (!ft_strncmp(commands[*i], ">", -1) )
	{
		check_permision(NULL, commands[*i + 1], 3);
		t_cmd_redir *redir =ft_new_redir(OUTPUT, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<", -1) )
	{
		check_permision(NULL, commands[*i + 1], 2);
		t_cmd_redir *redir =ft_new_redir(INPUT, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], ">>", -1))
	{
		check_permision(NULL, commands[*i + 1], 3);
		t_cmd_redir *redir =ft_new_redir(APPEND, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<<", -1))
	{
		char *heredoc_file = get_herdoc_data(commands[++(*i)]);
		t_cmd_redir *redir =ft_new_redir(HEREDOC, heredoc_file);
		add_redir_in_back(&info->redir, redir);

	}
}

void	parse_redirections(t_fill_info *info, char **commands)
{
	int	i;

	i = -1;
	while (commands[++i])
	{
		rederiction_error(commands, i);
		red_help(info, commands, &i);
	}
}
