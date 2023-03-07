/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 21:21:54 by ykhayri           #+#    #+#             */
/*   Updated: 2023/03/07 20:09:38 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **commands)
{
	int	i;
	int	counter;

	counter = 0;
	while (commands[counter])
		counter++;
	i = 0;
	while (commands[i])
	{
		if (!ft_strncmp(commands[i], ">", -1)
			|| !ft_strncmp(commands[i], "<", -1)
			||!ft_strncmp(commands[i], ">>", -1)
			|| !ft_strncmp(commands[i], "<<", -1))
		{
			counter -= 2;
			i++;
		}
		i++;
	}
	return (counter);
}

char	**make_new_args(char **commands)
{
	char	**args;
	int		k[2];

	args = my_alloc((count_args(commands) + 1) * sizeof(char *));
	k[i] = 0;
	k[j] = 0;
	if (!commands)
		return (NULL);
	while (commands[k[i]])
	{
		if (!ft_strncmp(commands[k[i]], ">", -1)
			|| !ft_strncmp(commands[k[i]], "<", -1)
			||!ft_strncmp(commands[k[i]], ">>", -1)
			|| !ft_strncmp(commands[k[i]], "<<", -1))
			k[i]++;
		else
		{
			args[k[j]] = commands[k[i]];
			k[j]++;
		}
		k[i]++;
	}
	args[k[j]] = NULL;
	return (args);
}

void	remove_spaces_in_between(t_args *vars)
{
	int	i;

	i = -1;
	while (vars->initial_commands[++i])
		vars->initial_commands[i] = ft_strtrim(vars->initial_commands[i], " ");
}

void	parse_commands(t_args *vars, char *line)
{
	vars->initial_commands = initial_split(vars, line);
	if (!vars->initial_commands)
		return ;
	remove_spaces_in_between(vars);
	parsing_commands(vars, vars->initial_commands);
}
