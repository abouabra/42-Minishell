/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:30:46 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 18:34:04 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/get_next_line.h"
#include "minishell.h"
#include <sys/signal.h>

void    init_termio(t_args *vars)
{
    tcgetattr(STDIN_FILENO, &vars->old_term);
    vars->new_term = vars->old_term;
    vars->new_term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &vars->new_term);
}
