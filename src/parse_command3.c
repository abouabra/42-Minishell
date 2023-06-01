/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 21:21:54 by ykhayri           #+#    #+#             */
/*   Updated: 2023/06/01 22:39:50 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		// if(!commands[i][0])
		// {
		// 	counter--;
		// 	i++;
		// }
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

char	**remove_empty_args(char **commands)
{
	int i = -1;
	char *total = "";
	while (commands[++i])
	{
		total = ft_strjoin(total, commands[i]);
		total = ft_strjoin(total, " ");
	}
	return split_command(total);
}

char	**make_new_args(char **commands)
{
	char	**args;
	int		k[2];

	args = my_alloc((count_args(commands) + 1) * sizeof(char *));
	// printf("count: %d\n", count_args(commands));
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

void	remove_spaces_in_between()
{
	int	i;

	i = -1;
	while (vars->initial_commands[++i])
		vars->initial_commands[i] = ft_strtrim(vars->initial_commands[i], " ");
}

int	has_char(char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (1);
	return (0);
}

void	parse_commands(char *line)
{
	vars->initial_commands = initial_split( line, 0);
	if (!vars->initial_commands)
	{
		return ;
	}
	remove_spaces_in_between();
	if(!parsing_commands( vars->initial_commands))
		return;
	
	execution_phase();
}
