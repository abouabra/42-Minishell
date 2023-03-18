/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:57 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 16:18:17 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_ter(t_args *vars)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
	{
		ft_dprintf(1, "\n");
		custom_exit(vars->exit_status);
	}
	if (line && line[0])
	{
		add_history(line);
		parse_commands(vars, line);
		// debug_menu(vars);
		vars->command_head = NULL;
	}
}

int	main(int ac, char **av, char **ev)
{
	t_args	*vars;

	(void) ac;
	(void) av;
	vars = my_alloc(sizeof(t_args));
	if (!vars)
		return (0);
	vars->ev = ev;
	set_env(vars);
	init_signal();
	while (1)
		start_ter(vars);
	custom_exit(0);
	return (0);
}
