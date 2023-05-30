/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/30 18:36:22 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

static void	check_permision_help(char *command_path, char *name, int a)
{
	if (a == 1)
	{
		if (!command_path)
		{
			// char *tmp = get_command_path(get_path(vars->backup_path), name);
			if(!get_env_data("PATH") || ft_strchr(name, '/'))
				ft_dprintf(2, "minishell: %s: No such file or directory\n", name);
			else
				ft_dprintf(2, "minishell: %s: command not found\n", name);
			// else if (ft_strchr(name, '/'))
			// 	ft_dprintf(2, "minishell: %s: No such file or directory\n", name);

		}
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

char	**expand_variables(t_fill_info *info, char **args)
{
	int		n[4];
	char	*strings[4];

	n[i] = -1;
	n[3] = -1;
	while (args[++n[i]])
	{
		strings[str] = ft_strchr(args[n[i]], '$');
		if (strings[str] && info->quote_type != 1)
			dollar_active(info, n, strings, args);
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

static void	retrieve_comm(t_fill_info *in, char **a[3])
{
	a[args] = expand_variables(in, a[args]);
	char *command_path = get_command_path(a[path], a[args][0]);

	// printf("command: |%s|\n", a[args][0]);
	if (a[args] && a[args][0])
	{
		if (check_permision(command_path, a[args][0], 1))
			in->is_valid_command = 0;
		else
			in->is_valid_command = 1;
	}
	in->command_path = command_path;
	in->command_args = a[args];
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
		if(!remove_quotes( info, a[arr]))
			return 0;
		
		// int i = -1;
		// while (a[arr][++i])
		// 	printf("a[arr][%d]: |%s|\n", i, a[arr][i]);
		// printf("\n\n\n\n");
		
		if(!parse_redirections(info, &a[arr]))
			return 0;
		a[args] = make_new_args(a[arr]);
		retrieve_comm(info, a);
		node = ft_new_command(info);
		add_command_in_back(&vars->command_head, node);
	}
	return 1;
}
