/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herd_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 20:57:53 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/19 21:04:01 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_herd_help(char *original_string, char **final, int *i)
{
	char	*data;
	int		j;
	char	*id;

	if (original_string[(*i)] == '?')
	{
		data = ft_itoa(g_vars->ex_status);
		*final = ft_strjoin((*final), data);
	}
	else
	{
		j = -1;
		while (original_string[++j + (*i)])
		{
			if (ft_strchr(" \"'\n", original_string[j + (*i)]))
				break ;
		}
		id = ft_substr(original_string, (*i), j);
		data = get_env_data(id);
		if (!data)
			data = "";
		(*final) = ft_strjoin((*final), data);
		(*i) += j - 1;
	}
}

char	*expand_herd(char *original_string, int should_expand)
{
	int		i;
	char	*final;
	char	tmp[2];

	final = "";
	i = -1;
	while (original_string[++i])
	{
		if (original_string[i] == '$' && should_expand)
		{
			i++;
			expand_herd_help(original_string, &final, &i);
		}
		else
		{
			tmp[0] = original_string[i];
			tmp[1] = '\0';
			final = ft_strjoin(final, tmp);
		}
	}
	return (final);
}

char	*expand_env(t_fill_info *info, char *str)
{
	int		should_expand;
	char	*tt;

	if (!str || !ft_strchr(str, '$'))
		return (str);
	should_expand = 1;
	if (info->quote_type == 1 || info->quote_type == 2)
		should_expand = 0;
	tt = expand_herd(str, should_expand);
	return (tt);
}

void	get_herdoc_data_help(char **total, t_fill_info *info, char *limiter)
{
	char	*str;
	char	*ww;

	while (1)
	{
		if (!g_vars->interrupted_mode && isatty(STDIN_FILENO))
			ft_dprintf(1, "> ");
		str = get_next_line(g_vars->heredocs_fd);
		if (g_vars->interrupted_mode == 3)
		{
			g_vars->ex_status = 1;
			*total = "";
			break ;
		}
		ww = expand_env(info, str);
		if (!ww || (ww && !ft_strncmp(ww, limiter, -1)))
			break ;
		*total = ft_strjoin((*total), ww);
	}
	g_vars->is_running = 0;
}

char	*get_herdoc_data(t_fill_info *info, char *limiter)
{
	char	*total;
	char	*name;
	int		fd;

	limiter = expand_env(info, ft_strtrim(limiter, "\"\'"));
	limiter = ft_strjoin(limiter, "\n");
	total = "";
	g_vars->is_running = 3;
	if (!isatty(0))
		g_vars->heredocs_fd = 0;
	else
		g_vars->heredocs_fd = dup(0);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->new_term);
	get_herdoc_data_help(&total, info, limiter);
	if (isatty(0))
		close(g_vars->heredocs_fd);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->old_term);
	name = "/tmp/herdoc_data";
	unlink(name);
	fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	write(fd, total, ft_strlen(total));
	close(fd);
	return (name);
}
