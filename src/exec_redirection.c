/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 21:43:07 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/19 21:51:37 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_redirections_help(t_cmd_redir *redir)
{
	int	fd;

	if (redir->type == APPEND)
	{
		fd = open(redir->file, O_RDWR | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
			custom_exit(1);
		dup2(fd, 1);
		close(fd);
	}
	if (redir->type == HEREDOC)
	{
		fd = open("/tmp/herdoc_data", O_RDONLY);
		if (fd == -1)
			custom_exit(1);
		dup2(fd, 0);
		close(fd);
	}
}

void	do_redirections(t_cmd_redir *head)
{
	t_cmd_redir	*redir;
	int			fd;

	redir = head;
	while (redir)
	{
		if (redir->type == INPUT)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 0);
			close(fd);
		}
		if (redir->type == OUTPUT)
		{
			fd = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd == -1)
				custom_exit(1);
			dup2(fd, 1);
			close(fd);
		}
		do_redirections_help(redir);
		redir = redir->next;
	}
}

int	has_char(char *s, char c)
{
	int	i;
	int	quote[2];

	quote[sin] = 0;
	quote[doub] = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == '\'' && !quote[doub])
			quote[sin] = !quote[sin];
		if (s[i] == '\"' && !quote[sin])
			quote[doub] = !quote[doub];
		if (s[i] == c && !quote[doub] && !quote[sin])
			return (1);
	}
	return (0);
}
