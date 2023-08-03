/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:11:02 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/02 23:21:52 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	doub_sin_skip(int *sin, int *doub, char *s, int i)
{
	if (s[i] == '\'')
		if (!*(doub))
			*(sin) = !*(sin);
	if (s[i] == '"')
		if (!*(sin))
			*(doub) = !*(doub);
}

char *are_two_args_number(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[++i])
	{
		j = -1;
		while (str[i][++j])
		{
			if (str[i][j] > '9' || str[i][j] < '0')
				return (&str[i][j]);
		}
	}
	return (0);
}

char	*is_arg_number(char *str)
{
	int	j;

	j = -1;
	while (str[++j])
	{
		if(str[j] == '-' && str[j + 2] == '\0')
			return (0);
		if (str[j] > '9' || str[j] < '0')
			return (&str[j]);
	}
	return (0);
}

void	fd_handler(int i)
{
	if (i > 0)
	{
		close(vars->prev_pipefd[0]);
		close(vars->prev_pipefd[1]);
	}
	if (i < vars->command_count - 1)
	{
		vars->prev_pipefd[0] = vars->next_pipefd[0];
		vars->prev_pipefd[1] = vars->next_pipefd[1];
	}
}
