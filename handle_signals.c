/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:30:46 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 18:44:11 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    init_termio(t_args *vars)
{
    tcgetattr(STDIN_FILENO, &vars->old_term);
    vars->new_term = vars->old_term;
    vars->new_term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &vars->new_term);
}
