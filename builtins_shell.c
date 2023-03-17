/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_shell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:55:49 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 00:01:58 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	the_search_built(t_env *search)
{
	while (search)
	{
		if (!ft_strncmp(search->env_id, "PWD", -1))
		{
			search->env_data = getcwd(NULL, -1);
			break ;
		}
		search = search->next;
	}
}

void	cd(t_args *vars, t_command *command)
{	
	t_env	*search;
	char	*data;
	int		ofsset;
	char	*new;

	if (!command->command_args[1])
		command->command_args[1] = get_env_data(vars, "HOME");
	else if (ft_strchr(command->command_args[1], '~'))
	{
		data = get_env_data(vars, "HOME");
		ofsset = ft_strchr_num(command->command_args[1], '~');
		new = ft_substr(command->command_args[1], ofsset +1,
				ft_strlen(command->command_args[1]));
		command->command_args[1] = ft_strjoin(data, new);
	}
	else if (access(command->command_args[1], F_OK))
	{
		ft_dprintf(2, "minishell: cd: %s: No such file or directory\n",
			command->command_args[1]);
		return ;
	}
	chdir(command->command_args[1]);
	search = vars->env_head;
	the_search_built(search);
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
	if (!ft_strncmp("-n", command->command_args[1], -1))
	{
		is_arg = 0;
		i++;
	}
	while (command->command_args[++i])
	{
		printf("%s", command->command_args[i]);
		if (command->command_args[i +1])
			printf(" ");
	}
	if (is_arg)
		printf("\n");
}

void	my_exit(char *arg)
{
	int	status;

	if (!arg)
		status = 0;
	else
		status = ft_atoi(arg);
	printf("exit\n");
	custom_exit(status);
}

void	pwd(void)
{
	printf("%s\n", getcwd(NULL, -1));
}
