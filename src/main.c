/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:57 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/03 02:58:32 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_var[3];
t_args	*vars;

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_var[ex_status] = 130;
		g_var[is_interupted] = 1;
		if (g_var[is_running])
		{
			g_var[is_interupted] = 0;
			g_var[ex_status] = 1;
			rl_redisplay();
		}
	}
	if (signum == SIGQUIT)
	{
		if (!g_var[is_running])
		{
			printf("Quit: 3\n");
			g_var[ex_status] = 131;
			g_var[is_interupted] = 1;
		}
	}
}

void	execute(t_command *tmp, int *index)
{
	// int		status;
	int i;
	int status;
	
	i = *index;
	if (i > 0 && vars->op[(i - 1) * 2] == '2'){
		
		int j = -1;
		while (++j < i)
			waitpid(vars->pid[j], &status, 0);
		*(vars->ex_status) = WEXITSTATUS(status);

		
		if (*vars->ex_status != 0 && vars->op[(i - 1) * 2 + 1] == '&')
		{
			while (vars->op[(i - 1) * 2] && ((vars->op[(i - 1) * 2] == '1' && vars->op[(i - 1) * 2 + 1] == '|') || (vars->op[(i - 1) * 2] == '2' && vars->op[(i - 1) * 2 + 1] != '|')))
				i++;
		}
		if (*vars->ex_status == 0 && vars->op[(i - 1) * 2 + 1] == '|')
		{
			while (vars->op[(i - 1) * 2] && ((vars->op[(i - 1) * 2] == '1' && vars->op[(i - 1) * 2 + 1] == '|') || (vars->op[(i - 1) * 2] == '2' && vars->op[(i - 1) * 2 + 1] != '&')))
				i++;
		}
		*index = i;
		if (i == vars->command_count)
			return;
	}
	if (i < vars->command_count - 1 && vars->op[i * 2] == '1')
		pipe(vars->next_pipefd);
	vars->pid[i] = fork();
	if (vars->pid[i] == -1)
		return ;
	if (vars->pid[i] == 0)
		handle_child(tmp, i);
	if ((i == 0 && vars->op[i * 2] == '1') || (i > 0 && vars->op[(i - 1) * 2] == '1'))
		fd_handler(i);
	if (g_var[is_interupted] && (g_var[ex_status] == 130
			|| g_var[ex_status] == 131))
	{
		*(vars->ex_status) = g_var[ex_status];
		g_var[is_interupted] = 0;
	}
	// else
	// 	*(vars->ex_status) = WEXITSTATUS(status);
}

void	execution_phase()
{
	t_command	*tmp;
	int			i;
	int			status;
	
	vars->pid = my_alloc(sizeof(int) * vars->command_count);
	tmp = vars->command_head;
	i = -1;
	while (++i < vars->command_count)
	{
		if (!tmp->command_args[0])
			tmp->is_valid_command = 69;
		if (tmp->command_args[0] && built_in_should_execute_in_main(tmp) && (!vars->op[0] || (vars->op[0] && (i - 1 >= 0 && vars->op[(i - 1) * 2] != '1') && (vars->op[i * 2] && vars->op[i * 2] != '1'))))
			execute_built_in(tmp);
		else
			execute( tmp, &i);
		tmp = tmp->next;
	}
	if(!built_in_should_execute_in_main(vars->command_head))
	{
		i = -1;
		while (++i < vars->command_count)
			waitpid(vars->pid[i], &status, 0);
		*(vars->ex_status) = WEXITSTATUS(status);
	}
}

void	start_ter()
{
	char	*line;

	if(isatty(STDIN_FILENO))
	{	
		line = readline("minishell> ");
		garbage_collector(line, 0);
	}
	else
		line = ft_strtrim(get_next_line(0)," \t\n\v\f\r");
	g_var[is_running] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->old_term);
	if (!line)
	{
		// ft_dprintf(1, "exit\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &vars->old_term);
		custom_exit(*vars->ex_status);
	}
	if (line && line[0])
	{
		add_history(line);
		parse_commands(line);
		vars->command_head = NULL;
	}
	g_var[is_running] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->new_term);
}

int	main(int ac, char **av, char **ev)
{

	(void)ac;
	(void)av;
	vars = my_alloc(sizeof(t_args));
	if (!vars)
		return (0);
	vars->ev = ev;
	set_env();
	init_termio();
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
	g_var[is_running] = 1;
	vars->is_running = &g_var[is_running];
	vars->ex_status = &g_var[ex_status];
	vars->is_interupted = &g_var[is_interupted];
	while (1)
		start_ter();
	custom_exit(0);
	return (0);
}
