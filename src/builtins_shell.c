/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_shell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:55:49 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/02 16:56:17 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	the_search_built(t_env *search, char *old_path)
{
	while (search)
	{
		if (!ft_strncmp(search->env_id, "PWD", -1))
			search->env_data = getcwd(NULL, 0);
		else if (!ft_strncmp(search->env_id, "OLDPWD", -1))
			search->env_data = old_path;
		search = search->next;
	}
}

void	cd(t_command *command)
{
	t_env	*search;
	char	*data;
	int		ofsset;
	char	*new;

	if (!command->command_args[1])
		command->command_args[1] = get_env_data( "HOME");
	if(!command->command_args[1])
	{
		ft_dprintf(2, "minishell: cd: HOME not set\n");
		*vars->ex_status = 1;
		return ;
	}
	else if (ft_strchr(command->command_args[1], '~'))
	{
		data = get_env_data( "HOME");
		ofsset = ft_strchr_num(command->command_args[1], '~');
		new = ft_substr(command->command_args[1], ofsset + 1,
				ft_strlen(command->command_args[1]));
		command->command_args[1] = ft_strjoin(data, new);
	}
	else if (access(command->command_args[1], F_OK))
	{
		ft_dprintf(2, "minishell: cd: %s: No such file or directory\n",
			command->command_args[1]);
		*vars->ex_status = 1;
		return ;
	}
	char *old_path = getcwd(NULL, 0);
	chdir(command->command_args[1]);
	search = vars->env_head;
	the_search_built(search,old_path);
}

void	echo(t_command *command)
{
	int	is_arg;
	int	i;

	is_arg = 1;
	i = 0;
	if (!command->command_args[1])
	{
		printf("\n");
		return ;
	}
	while (!ft_strncmp("-n", command->command_args[i +1], -1)
		|| !ft_strncmp("-nn", command->command_args[i +1], -1))
	{
		is_arg = 0;
		i++;
	}
	while (command->command_args[++i])
	{
		printf("%s", command->command_args[i]);
		if (command->command_args[i + 1])
			printf(" ");
	}
	if (is_arg)
		printf("\n");
}

void					my_exit(t_command *command)
{
	int	status = 0;

	if (!command->command_args[1])
		status = *vars->ex_status;
	else if(command->command_args[1] && command->command_args[2])
	{
		if((is_arg_number(command->command_args[1]) && !is_arg_number(command->command_args[2]))
		|| (is_arg_number(command->command_args[1]) && is_arg_number(command->command_args[2])))
		{
			status = 255;
			ft_dprintf(2,"minishell: exit: %s: numeric argument required\n",are_two_args_number(command->command_args));
		}
		else
		{
			status = 1;
			ft_dprintf(2,"minishell: exit: too many arguments\n");
		}
	}
	else
	{
		if(is_arg_number(command->command_args[1]))
		{
			ft_dprintf(2,"minishell: exit: %s: numeric argument required\n",is_arg_number(command->command_args[1]));
			status = 255;
		}
		else
			status = ft_atoi(command->command_args[1]);
	}
	custom_exit(status);
}

void	pwd(void)
{
	ft_dprintf(1,"%s\n", getcwd(NULL, 0));
	*vars->ex_status = 0;
}
