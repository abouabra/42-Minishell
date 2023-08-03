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
#include <stdio.h>
#include <sys/_types/_pid_t.h>
#include <sys/fcntl.h>
#include <unistd.h>

// int		g_var[3];
t_args	*vars;

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		vars->interrupted_mode = 1;
		// if(vars->is_running == 1)
		// 	printf("^C");
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		vars->is_interrupted = 1;
		if(vars->is_running == 3)
		{
			vars->interrupted_mode = 3;
			close(vars->heredocs_fd);
		}
		if(vars->is_running == 0)
		{
			vars->is_interrupted = 0;
			// g_var[is_interrupted] = 0;
			vars->ex_status = 1;
			// g_var[ex_status] = 1;
			rl_redisplay();
		}
	}
	if (signum == SIGQUIT)
	{
		vars->interrupted_mode = 2;
		// if (!g_var[is_running])
		if(vars->is_running)
		{
			if(vars->is_running != 3)
				printf("Quit: 3\n");
			// g_var[ex_status] = 131;
			vars->is_interrupted = 1;
			// g_var[is_interrupted] = 1;
			return;
		}
		vars->is_interrupted = 0;
	}
}

void	execute(t_command *tmp, int *index)
{
	// int		status;
	int i;
	int status;
	
	i = *index;
	if (i > 0 && vars->op[(i - 1) * 2] == '2')
	{
		int j = -1;
		while (++j < i)
			waitpid(vars->pid[j], &status, 0);
		(vars->ex_status) = WEXITSTATUS(status);		
		if (vars->ex_status != 0 && vars->op[(i - 1) * 2 + 1] == '&')
		{
			while (vars->op[(i - 1) * 2] && ((vars->op[(i - 1) * 2] == '1' && vars->op[(i - 1) * 2 + 1] == '|') || (vars->op[(i - 1) * 2] == '2' && vars->op[(i - 1) * 2 + 1] != '|')))
				i++;
		}
		if (vars->ex_status == 0 && vars->op[(i - 1) * 2 + 1] == '|')
		{
			while (vars->op[(i - 1) * 2] && ((vars->op[(i - 1) * 2] == '1' && vars->op[(i - 1) * 2 + 1] == '|') || (vars->op[(i - 1) * 2] == '2' && vars->op[(i - 1) * 2 + 1] != '&')))
				i++;
		}
		*index = i;
		if (i == vars->command_count)
			return;
	}
	// check permisions for redir
	t_cmd_redir *redir = tmp->redir;
	while(redir)
	{
		//check if file exists using check_permision function
		if (redir->type == OUTPUT)
			check_permision(NULL, redir->file, 3);
		else if (redir->type == INPUT)
			check_permision(NULL, redir->file, 2);
		else if (redir->type == APPEND)
			check_permision(NULL, redir->file, 3);
		redir = redir->next;
	}

	// check permisions for command
	if (tmp->command_args && tmp->command_args[0])
	{
		if (check_permision(tmp->command_path, tmp->command_args[0], 1))
			tmp->is_valid_command = 0;
		else
			tmp->is_valid_command = 1;
	}


	// if (i < vars->command_count - 1 && vars->op[i * 2] == '1')
	if (i < vars->command_count - 1 && (!vars->op[0] || (vars->op[0] && vars->op[i * 2] == '1')))
		pipe(vars->next_pipefd);
	vars->pid[i] = fork();
	if (vars->pid[i] == -1)
		return ;
	if (vars->pid[i] == 0)
		handle_child(tmp, i);
	if ((i == 0 && vars->op[i * 2] == '1') || (i > 0 && (vars->op[(i - 1) * 2] == '1' || vars->op[(i) * 2] == '1')))
		fd_handler(i);
	
	// else
	// 	(vars->ex_status) = WEXITSTATUS(status);
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
	tmp = vars->command_head;
	i=-1;
	while (++i < vars->command_count)
	{
		if(!built_in_should_execute_in_main(tmp) || (built_in_should_execute_in_main(tmp) && ((vars->op[0] && ((i - 1 >= 0 && vars->op[(i - 1) * 2] == '1') || (vars->op[i * 2] && vars->op[i * 2] == '1'))))))
		{
			int j = -1;
			while (++j <= i)
				waitpid(vars->pid[j], &status, 0);
			(vars->ex_status) = WEXITSTATUS(status);
		}
		tmp = tmp->next;
	}
	if(vars->is_interrupted)
	{
		// g_var[is_interrupted] = 0;
		if(vars->interrupted_mode == 1)
			vars->ex_status = 130;
		if(vars->interrupted_mode == 2)
			vars->ex_status = 131;
		if(vars->interrupted_mode == 3)
			vars->ex_status = 1;
		vars->is_interrupted = 0;
		vars->interrupted_mode = 0;
		// g_var[ex_status] = vars->ex_status;
		// g_var[ex_status] = vars->ex_status;
	}
	// printf("interrupted_mode %d || is interupted %d || is_running %d\n", vars->interrupted_mode, vars->is_interrupted,vars->is_running);
}

void	start_ter()
{
	char	*line;

	vars->is_running = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->new_term);
	if(isatty(STDIN_FILENO))
	{	
		line = readline("minishell> ");
		garbage_collector(line, 0);
	}
	else
		line = ft_strtrim(get_next_line(0)," \t\n\v\f\r");
	// g_var[is_running] = 0;
	vars->is_running = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->old_term);
	if (!line)
	{
		// ft_dprintf(1, "exit\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &vars->old_term);
		custom_exit(vars->ex_status);
	}
	if (line && line[0])
	{
		add_history(line);
		parse_commands(line);
		vars->command_head = NULL;
	}
	// g_var[is_running] = 1;
	
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
	// g_var[is_running] = 1;
	vars->is_running = 0;
	// vars->is_running = &g_var[is_running];
	// vars->ex_status = &g_var[ex_status];
	// vars->is_interrupted = &g_var[is_interrupted];
	while (1)
		start_ter();
	custom_exit(0);
	return (0);
}
