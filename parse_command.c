/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/17 23:51:17 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_dprintf.h"
#include "libft/get_next_line.h"
#include "libft/libft.h"
#include "minishell.h"

static void	check_permision_help(char *command_path, char *name, int a)
{
	if (a == 1)
	{
		if (!command_path)
			ft_dprintf(2, "minishell: %s: command not found\n", name);
		else if (command_path && access(command_path, X_OK) == -1)
			ft_dprintf(2, "minishell: %s: Permission denied\n", name);
	}
	else
	{
		if (access(name, F_OK) == -1)
			ft_dprintf(2, "minishell: %s: No such file or directory\n", name);
		else if (access(name, R_OK) == -1)
			ft_dprintf(2, "minishell: %s: Permission denied\n", name);
	}
}

int	check_permision(char *command_path, char *name, int arg)
{
	if (is_built_in(name))
		return (0);
	if (arg == 1)
	{
		check_permision_help(command_path, name, 1);
		if (!command_path || (command_path && access(command_path, X_OK) == -1))
			return (1);
	}	
	if (arg == 2)
	{
		check_permision_help(command_path, name, 2);
		if (access(name, F_OK) == -1 || access(name, R_OK) == -1)
			return (1);
	}
	if (arg == 3)
	{
		if (access(name, F_OK) != -1 && access(name, W_OK) == -1)
		{
			ft_dprintf(2, "minishell: %s: Permission denied\n", name);
			return (1);
		}
	}
	return (0);
}

void	expand_variables(t_args *vars, t_fill_info *info, char **args)
{
	int		n[3];
	char	*data;
	char	*str;
	char	*new;
	char	*tmp;

	n[i] = -1;
	while (args[++n[i]])
	{
		str = ft_strchr(args[n[i]], '$');
		if (str && info->quote_type != 1)
		{
			n[k] = -1;
			while (str[++n[k]] && str[n[k]] != '\'' && str[n[k]] != '"' && str[n[k]] != ' ')
				;
			if (!str[n[k]])
				n[k]++;
			tmp = ft_substr(str, 1, n[k] - 1);
			data = get_env_data(vars, tmp);
			n[j] = ft_strchr_num(args[n[i]], '$');
			new = ft_substr(args[n[i]], 0, n[j]);
			
			if (!ft_strncmp(str + 1, "?", -1))
				args[n[i]] = ft_strjoin(new, ft_itoa(vars->exit_status));
			else
			{
				if (data)
				{
					args[n[i]] = ft_strjoin(new, data);
					args[n[i]] = ft_strjoin(args[n[i]], ft_substr(str, n[k], -1));
				}
				else
					args[n[i]] = ft_strjoin(new, "");
			}
		}
	}
}

static void	retrieve_comm(char *c_p, t_fill_info *in, char **a[3], t_args *v)
{
	if (a[args] && a[args][0])
	{
		if (check_permision(c_p, a[args][0], 1))
			in->is_valid_command = 0;
		else
			in->is_valid_command = 1;
	}
	expand_variables(v, in, a[args]);
	in->command_path = c_p;
	in->command_args = a[args];
}

void	parsing_commands(t_args *vars, char **commands)
{
	int			i;
	char		**a[3];
	char		*command_path;
	t_command	*node;
	t_fill_info	*info;

	info = my_alloc(sizeof(t_fill_info));
	i = -1;
	a[path] = get_path(vars);
	while (commands[++i])
	{
		if (!commands[i][0])
			continue ;
		ft_memset(info, 0, sizeof(t_fill_info));
		a[arr] = split_command(commands[i]);
		remove_quotes(vars, info, a[arr]);
		parse_redirections(info, a[arr]);
		a[args] = make_new_args(a[arr]);
		command_path = get_command_path(a[path], a[args][0]);
		retrieve_comm(command_path, info, a, vars);
		node = ft_new_command(info);
		add_command_in_back(&vars->command_head, node);
	}
}
