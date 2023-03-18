/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:11:02 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/18 16:56:24 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	doub_sin_skip(int *sin, int *doub, char *s, int i)
{
	if (s[i] == '\'')
		if (!*(doub))
			*(sin) = !*(sin);
	if (s[i] == '"')
		if (!*(sin))
			*(doub) = !*(doub);
}

void	print_command(t_command *command)
{
	int	i;

	printf("COMMAND_PATH: %s\n", command->command_path);
	i = -1;
	while (command->command_args[++i])
		printf("\tSUB COMMAND: %s\n", command->command_args[i]);
	printf("QUOTE_TYPE: %d\n", command->quote_type);
	printf("IS_INPUT: %d\n", command->is_input);
	printf("INPUT_FILE: %s\n", command->input_file);
	printf("IS_OUTPUT: %d\n", command->is_output);
	printf("OUTPUT_FILE: %s\n", command->output_file);
	printf("IS_APPEND: %d\n", command->is_append);
	printf("APPEND_FILE: %s\n", command->append_file);
	printf("IS_HERDOC: %d\n", command->is_herdoc);
	printf("HERDOC_DATA: %s\n", command->herdoc_data);
	printf("HERDOC_LIMITER: %s\n", command->herdoc_limiter);
	printf("\n");
}

void	debug_menu(t_args *vars)
{
	t_command	*tst;

	tst = vars->command_head;
	while (tst)
	{
		print_command(tst);
		tst = tst->next;
	}
	ft_dprintf(1, "EXIT_CODE: %d\n", vars->exit_status);
}
