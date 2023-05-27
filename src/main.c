/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:57 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/27 23:17:11 by abouabra         ###   ########.fr       */
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

void	execute(t_command *tmp, int i)
{
	pid_t	pid;
	int		status;
	int		fd;

	fd = 0;
	if (i < vars->command_count - 1)
		pipe(vars->next_pipefd);
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		handle_child( tmp, fd, i);
	fd_handler( i);
	waitpid(pid, &status, 0);
	if (g_var[is_interupted] && (g_var[ex_status] == 130
			|| g_var[ex_status] == 131))
	{
		*(vars->ex_status) = g_var[ex_status];
		g_var[is_interupted] = 0;
	}
	else
		*(vars->ex_status) = WEXITSTATUS(status);
}

void	execution_phase()
{
	t_command	*tmp;
	int			i;

	tmp = vars->command_head;
	i = -1;
	while (++i < vars->command_count)
	{
		if (!tmp->command_args[0])
			tmp->is_valid_command = 69;
		if (tmp->command_args[0] && built_in_should_execute_in_main(vars, tmp))
			execute_built_in( tmp);
		else
			execute(tmp, i);
		tmp = tmp->next;
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
		parse_commands( line);
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
