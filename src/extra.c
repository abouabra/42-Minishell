/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:11:02 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/26 22:14:01 by abouabra         ###   ########.fr       */
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

int	is_all_numbers(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (0);
	}
	return (1);
}

void	fd_handler(t_args *vars, int i)
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
