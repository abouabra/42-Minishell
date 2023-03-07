/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_help.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 19:52:24 by ykhayri           #+#    #+#             */
/*   Updated: 2023/03/06 19:53:40 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	the_search_env(t_env *search, char **args)
{
	while (search)
	{
		if (!ft_strncmp(search->env_id, args[0], -1))
		{
			search->env_data = args[1];
			break ;
		}
		search = search->next;
	}
}
