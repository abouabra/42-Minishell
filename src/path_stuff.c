/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:31:51 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/30 17:28:13 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_path(void)
{
	char	*path;

	path = get_env_data( "PATH");
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

	if (!access(command, F_OK))
		return (command);
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
