/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:57 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 20:10:25 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_args	*g_vars;

void	start_execution(t_command *tmp)
{
	int	i;
	int	j;

	i = -1;
	while (++i < g_vars->command_count)
	{
		if (!tmp->command_args[0])
			tmp->is_valid_command = 69;
		tmp->command_args = expand_variables(tmp->info, tmp->command_args);
		j = -1;
		while (tmp->command_args[++j])
			fix_string(tmp->info, tmp->command_args[j], tmp->command_args[j]);
		if (tmp->command_args[0] && built_in_should_execute_in_main(tmp)
			&& (!g_vars->op[0] || (g_vars->op[0] && (i - 1 >= 0
						&& g_vars->op[(i - 1) * 2] != '1') && (g_vars->op[i * 2]
						&& g_vars->op[i * 2] != '1')) || (g_vars->op[0]
					&& (i == 0 && (g_vars->op[i * 2]
							&& g_vars->op[i * 2] != '1')))))
			execute_built_in(tmp);
		else
			execute(&tmp, &i);
		if (tmp)
			tmp = tmp->next;
	}
}

void	fill_line(char **line)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->new_term);
	if (isatty(STDIN_FILENO))
	{
		(*line) = readline("minishell> ");
		garbage_collector((*line), 0);
	}
	else
		(*line) = ft_strtrim(get_next_line(0), " \t\n\v\f\r");
	g_vars->is_running = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->old_term);
	if (!(*line))
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->old_term);
		custom_exit(g_vars->ex_status);
	}
}

void	start_ter_help(char *line)
{
	char	*arr2[2];

	arr2[0] = line;
	arr2[1] = NULL;
	if (g_vars->prev_pipefd[0] != 0)
	{
		close(g_vars->prev_pipefd[0]);
		g_vars->prev_pipefd[0] = 0;
	}
	if (g_vars->prev_pipefd[1] != 0)
	{
		close(g_vars->prev_pipefd[1]);
		g_vars->prev_pipefd[1] = 0;
	}
	if (g_vars->next_pipefd[0] != 0)
	{
		close(g_vars->next_pipefd[0]);
		g_vars->next_pipefd[0] = 0;
	}
	if (g_vars->next_pipefd[1] != 0)
	{
		close(g_vars->next_pipefd[1]);
		g_vars->next_pipefd[1] = 0;
	}
	nested_par(arr2, 1, 0);
}

void	start_ter(void)
{
	char	*line;
	char	*arr[2];

	g_vars->is_running = 0;
	fill_line(&line);
	if (line && line[0])
	{
		add_history(line);
		arr[0] = line;
		arr[1] = NULL;
		g_vars->iter_else_count = 0;
		g_vars->iteration = 0;
		g_vars->op = "";
		if (nested_par(arr, 0, 0))
		{
			g_vars->op = "";
			g_vars->iter_else_count = 0;
			g_vars->iteration = 0;
			g_vars->pipe = 0;
			start_ter_help(line);
		}
		g_vars->command_head = NULL;
	}
}

int	main(int ac, char **av, char **ev)
{
	(void)ac;
	(void)av;
	g_vars = my_alloc(sizeof(t_args));
	if (!g_vars)
		return (0);
	g_vars->ev = ev;
	set_env();
	init_termio();
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
	g_vars->is_running = 0;
	while (1)
		start_ter();
	custom_exit(0);
	return (0);
}
