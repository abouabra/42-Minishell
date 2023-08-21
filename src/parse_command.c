/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/21 15:24:50 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	check_permision_help(char *command_path, char *name, int a)
{
	if (a == 1)
	{
		if (!command_path)
		{
			if (!get_env_data("PATH") || ft_strchr(name, '/'))
				ft_dprintf(2, "minishell: %s: No such file or directory\n",
					name);
			else
				ft_dprintf(2, "minishell: %s: command not found\n", name);
		}
		else if (command_path && access(command_path, X_OK) == -1)
			ft_dprintf(2, "minishell: %s: Permission denied\n", name);
	}
	else
	{
		if (access(name, F_OK) == -1)
		{
			ft_dprintf(2, "minishell: %s: No such file or directory\n", name);
			g_vars->ex_status = 1;
		}
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

char	**expand_variables(t_fill_info *info, char **args)
{
	int		n[4];
	char	*strings[4];

	n[i] = -1;
	n[3] = -1;
	while (args[++n[i]])
	{
		strings[str] = ft_strchr(args[n[i]], '$');
		if (strings[str])
			exp_var_quote(info, args, n, strings);
		else if (ft_strchr(args[n[i]], '*') && info->quote_type == 0)
		{
			strings[tmp] = "";
			while (args[++n[3]])
			{
				if (ft_strchr(args[n[3]], '*') && info->quote_type == 0
					&& wildcard(args[n[3]])[0])
					args[n[3]] = wildcard(args[n[3]]);
				strings[tmp] = ft_strjoin(strings[tmp], args[n[3]]);
				strings[tmp] = ft_strjoin(strings[tmp], " ");
			}
			args = make_new_args(split_command(strings[tmp]));
		}
	}
	return (args);
}

static int	retrieve_comm(t_fill_info *in, char **a[3])
{
	int		i;
	char	*command_path;

	if (!test_ambiguous(in, a[arr]))
		return (0);
	i = -1;
	while (a[arr][++i])
		red_help(in, a[arr], &i);
	a[args] = make_new_args(a[arr]);
	i = -1;
	while (a[args][++i])
		fix_string(in, a[args][i], a[args][i]);
	command_path = get_command_path(a[path], a[args][0]);
	in->command_path = command_path;
	in->command_args = a[args];
	return (1);
}

int	parsing_commands(char **commands)
{
	int			i;
	char		**a[3];
	t_command	*node;
	t_fill_info	*info;

	info = my_alloc(sizeof(t_fill_info));
	i = -1;
	a[path] = get_path();
	while (commands[++i])
	{
		if (!commands[i][0])
			continue ;
		ft_memset(info, 0, sizeof(t_fill_info));
		a[arr] = split_command(commands[i]);
		if (!remove_quotes(info, a[arr]))
			return (0);
		if (!retrieve_comm(info, a))
			return (0);
		node = ft_new_command(info);
		add_command_in_back(&g_vars->command_head, node);
	}
	return (1);
}
