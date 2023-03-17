/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:31:51 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/17 23:49:27 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>

char    **get_path(t_args *vars)
{
    char    *path;

    path = get_env_data(vars, "PATH");
    if (!path)
        path = "";
    return (ft_split(path, ':'));
}

char	*get_command_path(char **path, char *command)
{
	int		i;
	int		ret;
	char	*str;
	char	*stt;

	if(!access(command, F_OK))
		return command;
	i = -1;
	while (path[++i])
	{
		stt = ft_strrchr(command, '/');
		if (!stt)
			stt = command;
		else
			command = ++stt;
		str = ft_strjoin(ft_strjoin(path[i], "/"), command);
		ret = access(str, F_OK);
		if (!ret)
			return (str);
	}
	return (NULL);
}
