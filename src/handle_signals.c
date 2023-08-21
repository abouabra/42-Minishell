/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:30:46 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/20 20:09:23 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_termio(void)
{
	tcgetattr(STDIN_FILENO, &g_vars->old_term);
	g_vars->new_term = g_vars->old_term;
	g_vars->new_term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &g_vars->new_term);
}

int	handle_signals2(int signum)
{
	if (signum == SIGQUIT)
	{
		g_vars->interrupted_mode = 2;
		if (g_vars->is_running)
		{
			if (g_vars->is_running != 3)
				ft_dprintf(1, "Quit: 3\n");
			g_vars->is_interrupted = 1;
			return (0);
		}
		g_vars->is_interrupted = 0;
	}
	return (1);
}

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		g_vars->interrupted_mode = 1;
		ft_dprintf(1, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_vars->is_interrupted = 1;
		if (g_vars->is_running == 3)
		{
			g_vars->interrupted_mode = 3;
			close(g_vars->heredocs_fd);
		}
		if (g_vars->is_running == 0)
		{
			g_vars->is_interrupted = 0;
			g_vars->ex_status = 1;
			rl_redisplay();
		}
	}
	if (!handle_signals2(signum))
		return ;
}
