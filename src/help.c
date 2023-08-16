/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 19:52:24 by ykhayri           #+#    #+#             */
/*   Updated: 2023/06/01 21:09:46 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>

int	checker(char **commands, int i)
{
	int		j;
	char	*str;

	j = i;
	if (!ft_strncmp(commands[i], ">", -1))
		str = ft_strdup(">>");
	if (!ft_strncmp(commands[i], ">>", -1))
		str = ft_strdup(">");
	if (!ft_strncmp(commands[i], "<", -1))
		str = ft_strdup("<<");
	if (!ft_strncmp(commands[i], "<<", -1))
		str = ft_strdup("<");
	while (commands[++j])
	{
		if (!ft_strncmp(commands[j], commands[i], -1)
			|| !ft_strncmp(commands[j], str, -1))
			return (0);
	}
	return (1);
}

int rederiction_error(char **commands, int i)
{
	if (ft_strnstr(commands[i], "<<<", -1) || ft_strnstr(commands[i], ">>>", -1)
	|| ft_strnstr(commands[i], "=>", -1)
	|| (!ft_strncmp(commands[i], ">", -1) && commands[i + 1] && !ft_strncmp(commands[i + 1], "<", -1))
	|| (!ft_strncmp(commands[i], "<", -1) && commands[i + 1] && !ft_strncmp(commands[i + 1], ">", -1)))
	{
		ft_dprintf(2, "minishell: syntax error\n");
		vars->ex_status = 2;
		return 0;
	}
	if ((!ft_strncmp(commands[i], ">", -1) || !ft_strncmp(commands[i],
				"<", -1) || !ft_strncmp(commands[i], ">>", -1)
			|| !ft_strncmp(commands[i], "<<", -1)) && !commands[i + 1])
	{
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		vars->ex_status = 2;
		return(0);
	}
	if (commands[i + 1] && (!ft_strncmp(commands[i], ">", -1)
			|| !ft_strncmp(commands[i], "<", -1) || !ft_strncmp(commands[i],
				">>", -1) || !ft_strncmp(commands[i], "<<", -1)) && (
			!ft_strncmp(commands[i + 1], ">", -1)
			|| !ft_strncmp(commands[i + 1], "<", -1)
			|| !ft_strncmp(commands[i + 1], ">>", -1)
			|| !ft_strncmp(commands[i + 1], "<<", -1)))
	{
		ft_dprintf(1, "minishell: syntax error near unexpected token `%s'\n",
			commands[i + 1]);
		vars->ex_status = 2;
		return 0;
	}
	return 1;
}

void	dollar_active(t_fill_info *info, int n[4], char *strings[4], char **args)
{
	// ft_dprintf(1, "Start: |%s|   |%d|\n", strings[str],info->quote_type);
	int sould_remove_space = 0;
	if(info->quote_type == 0)
		sould_remove_space = 1;
	n[k] = 0;
	if(strings[str][n[k] -1] && strings[str][n[k] -1] == '\'')
	{
		info->quote_type = 1;
		return;	
	}
	while(strings[str][n[k]])
	{		
		if(!ft_strchr(strings[str], '$'))
			return;
		// ft_dprintf(1, "loop: |%s|\n",strings[str]);
		while (strings[str][++n[k]] && strings[str][n[k]] != '\''
			&& strings[str][n[k]] != '"' && strings[str][n[k]] != ' '
			&& strings[str][n[k]] != '-')
		{
			if(strings[str][n[k]] == '?')
			{
				n[k]++;
				break;
			}
			if(strings[str][n[k]] == '$')
				break;
			if(strings[str][n[k]] == '@')
			{
				n[k]++;
				break;
			}
			if(ft_isdigit(strings[str][n[k]]))
			{
				n[k]++;
				break;
			}
		}

		if (!strings[str][n[k]])
			n[k]++;
		strings[tmp] = ft_substr(strings[str], 1, n[k] - 1);

		if(!strings[tmp][0])
			strings[data] = "$";
		else if(strings[tmp][0] == '?')
			strings[data] = ft_itoa(vars->ex_status);
		else if(strings[tmp][0] == '@')
			strings[data] = "";
		else if(ft_isdigit(strings[tmp][0]))
		{
			strings[data] = args[ft_atoi(strings[tmp])];
			strings[data] = "";
		}
		else
			strings[data] = get_env_data( strings[tmp]);
		


		n[j] = ft_strchr_num(args[n[i]], '$');
		strings[news] = ft_substr(args[n[i]], 0, n[j]);
		// if(sould_remove_space)
			// strings[data] = ft_strtrim(strings[data], " ");
		

		// ft_dprintf(1, "substr: |%s|   %d   data: |%s|\n\n\n\n\n",strings[tmp],sould_remove_space,strings[data]);

		if (!ft_strncmp(strings[str] + 1, "?", -1))
			args[n[i]] = ft_strjoin(strings[news], ft_itoa((vars->ex_status)));
		else
		{
			if (strings[data])
			{
				args[n[i]] = ft_strjoin(strings[news], strings[data]);
				args[n[i]] = ft_strjoin(args[n[i]],
						ft_substr(strings[str], n[k], -1));
			}
			else
				args[n[i]] = ft_strjoin(strings[news], "");
		}
		
		strings[str] += n[k];
		n[k] = 0;
	}
}

static int im_i_in_9aws(char *arr)
{
	arr = ft_strtrim(arr, " \t\n");
	//check if arr has operators
	if(!ft_strncmp(arr, "|", 1) || !ft_strncmp(arr, "&&", 2)
		|| !ft_strncmp(arr + ft_strlen(arr) - 1, "|", 1) || !ft_strncmp(arr + ft_strlen(arr) - 2, "&&", 2))
		return 0;
	return 1;
}


static int	par_coount(char *s)
{
	int	i;
	int	active;
	int par_c;

	i = 0;
	active = 0;
	par_c = 0;
	while (s[i])
	{
		if (s[i] == '(')
			active++;
		if (s[i] == ')')
		{
			active--;
			if (!active)
				par_c++;
		}
		i++;
	}
	if (active)
		return (-1);
	return (par_c);
}
int	nested_par(char **arr, int check, int index)
{
	char	**tmp;
	int		i;
	int		j;
	
	// if(check)
	// 	printf("nested: %s\n",arr[0]);
	// static int iter_else_count;
	i = -1;
	int status;
	// printf("arr: %s %d %d\n",arr[0], vars->ex_status, index);
	while (arr[++i])
	{
		arr[i] = ft_strtrim(arr[i], " \t");
		
		
		
		if (has_char(arr[i], '(') || has_char(arr[i], ')'))
		{	
			// if(check)
			// if (arr[i][0] == '&' || arr[i][0] == '|')
			int parentheses_c = par_coount(arr[i]);
			if (!check && parentheses_c == -1)
			{
				ft_dprintf(1, "minishell: syntax error\n");
				vars->ex_status = 2;
				return (0);
			}
			// printf("test: %d  arr: %s\n",par_coount(arr[i]),arr[i]);
			char *save = ft_strdup (arr[i]);
			if(parentheses_c == 1 && arr[i][0] == '(' && arr[i][ft_strlen(arr[i]) - 1] == ')')
			{

								// printf("op: %s   arr[i]: |%s|    ex status: |%d|\n",vars->op,arr[i],vars->ex_status);
				if (check && arr[i -1] && (arr[i -1][ft_strlen(arr[i - 1]) - 1] == '&' || (arr[i - 1][ft_strlen(arr[i - 1]) - 1] == '|' && arr[i - 1][ft_strlen(arr[i - 1]) - 2] && arr[i - 1][ft_strlen(arr[i - 1]) - 2] == '|')))
				{
					if (vars->ex_status != 0 && vars->op[ft_strlen(vars->op) -1] == '&')
					{
						//REMINDER
						//  ls && gg || (pp || ps)
						// problem is that it will skip the second arr cuz op[0] is && even tho it should check ||
						//SKIP
						while(arr[i] && ((par_coount(arr[i]) == 1 && arr[i][0] == '(' && arr[i][ft_strlen(arr[i]) - 1] == ')' )|| !ft_strnstr(arr[i], "||", -1)))
						{
							if(arr[++i])
								arr[i] = ft_strtrim(arr[i], " \t");
						}
						if (arr[i])
						{
							if(ft_strlen(arr[i]) == 2)
								i++;
							if(!par_coount(arr[i]) && arr[i][0] != '(' && arr[i][ft_strlen(arr[i]) - 1] != ')')
								return nested_par(&arr[i], check, index +1);
						}
						// if (vars->op[0] && ((vars->op[0] == '1' && vars->op[0 + 1] == '|') || (vars->op[0] == '2' && vars->op[0 + 1] != '|')))
						// 	i++;
					}
					if (vars->ex_status == 0 && vars->op[ft_strlen(vars->op) -1] == '|')
					{
						// printf("2      arr: %s || ex status: %d      || op: %s\n",arr[i],vars->ex_status,vars->op);

						while(arr[i] && ((par_coount(arr[i]) == 1 && arr[i][0] == '(' && arr[i][ft_strlen(arr[i]) - 1] == ')' )|| !ft_strnstr(arr[i], "&&", -1)))
						{
							if(arr[++i])
								arr[i] = ft_strtrim(arr[i], " \t");
						}
						if (arr[i])
						{
							if(ft_strlen(arr[i]) == 2)
								i++;
							if(!par_coount(arr[i]) && arr[i][0] != '(' && arr[i][ft_strlen(arr[i]) - 1] != ')')
								return nested_par(&arr[i], check, index +1);
						}
						//SKIP
						// if (vars->op[0] && ((vars->op[0] == '1' && vars->op[0 + 1] == '|') || (vars->op[0] == '2' && vars->op[0 + 1] != '&')))
						// 	i++;
					}
				}





				if(!arr[i])
				{
					// printf("printf :%s\n",arr[i]);
					return 0;
				}






				arr[i] = ft_substr(arr[i], 1, ft_strlen(arr[i]) - 2);
				// printf("ta malk: %s\n", arr[i]);
				if(!arr[i][0])
				{
					ft_dprintf(1, "minishell: syntax error\n");
					vars->ex_status = 2;
					return (0);
				}
			}
			
			// printf("after substr arr: %s\n",arr[i]);
			
			char **kobi = split_par(arr[i]);
			int x = -1;
			while(!check && kobi[++x])
			{
				kobi[x] = ft_strtrim(kobi[x], " \t");
				// printf("kobi: |%s| x: %d\n",kobi[x],x);
				char **op_err = ft_split_charset(kobi[x], " \t");
				int y = -1;
				while(op_err[++y])
				{
					int w = -1;
					int op_c = 0;
					while(op_err[y][++w])
					{
						if (op_err[y][w] == '|' || op_err[y][w] == '&')
						{
							op_c++;
						}
						if (op_c > 2)
						{
							ft_dprintf(1, "minishell: syntax error\n");
							vars->ex_status = 2;	
							return 0;
						}
					}
					if(op_err[y + 1] && (op_err[y][ft_strlen(op_err[y]) - 1] == '&' || op_err[y][ft_strlen(op_err[y]) - 1] == '|')
					&& (op_err[y + 1][0] == '&' || op_err[y +1][0] == '|'))
					{
						ft_dprintf(1, "minishell: syntax error\n");
						vars->ex_status = 2;
						return (0);
					}
				}
				// if(par_coount(kobi[x]) == 1 && kobi[x][0] == '(' && kobi[x][ft_strlen(kobi[x]) - 1] == ')')
				if(kobi[x][0] == '&' || kobi[x][0] == '|')
				{
					if (!x || (x - 1 >= 0 && par_coount(kobi[x -1]) == 0))
					{
						ft_dprintf(1, "minishell: syntax error\n");
						vars->ex_status = 2;
						return (0);
					}
				}
				if (kobi[x][ft_strlen(kobi[x]) - 1] == '&' || kobi[x][ft_strlen(kobi[x]) - 1] == '|')
				{
					if (!kobi[x + 1] || (kobi[x + 1] && par_coount(kobi[x + 1]) == 0))
					{
						ft_dprintf(1, "minishell: syntax error\n");
						vars->ex_status = 2;
						return (0);
					}
				}
				if (par_coount(kobi[x]) == 1 && kobi[x][0] == '(' && kobi[x][ft_strlen(kobi[x]) - 1] == ')')
				{
					if (kobi[x + 1])
					{
						kobi[x + 1] = ft_strtrim(kobi[x+1], " \t");
						if (kobi[x + 1][0] != '&' && kobi[x + 1][0] != '|')
						{
							ft_dprintf(1, "minishell: syntax error\n");
							vars->ex_status = 2;
							return (0);
						}
					}
					if (x > 0)
					{
						kobi[x -1] = ft_strtrim(kobi[x - 1], " \t");
						if (kobi[x -1][ft_strlen(kobi[x - 1]) - 1] != '&' && kobi[x -1][ft_strlen(kobi[x - 1]) - 1] != '|')
						{
							ft_dprintf(1, "minishell: syntax error\n");
							vars->ex_status = 2;
							return (0);
						}
					}
				}
				// if(!kobi || (!operations(arr[i])[0] && kobi[1] && kobi[1][0]))
				// {
				// 	ft_dprintf(1, "minishell: syntax error\n");
				// 	vars->ex_status = 2;
				// 	return (0);
				// }
			}
			// printf("ok\n");
			// vars->iteration++;
			// printf("arr: |%s|     save: |%s|   index: %d     check: %d\n",arr[i],save,  (index == 0 && save[0] == '('), check);

			if(check && ( index > 0 || (index == 0 && save[0] == '(' && save[ft_strlen(save) - 1] == ')')))
			{
				// printf("ok\n");
				if(arr[i+1] && arr[i+1][0] && arr[i+1][0] == '|' && arr[i+1][1] && arr[i+1][1] != '|')
				{
					printf("in child && pipe after me arr: |%s| && arr+1: |%s|\n",arr[i],arr[i+1]);
				}
				// ft_dprintf(2,"prev 0: %d      prev 1: %d\n",vars->prev_pipefd[0],vars->prev_pipefd[1]);
				// ft_dprintf(2,"next 0: %d      next 1: %d\n",vars->next_pipefd[0],vars->next_pipefd[1]);
				close(vars->prev_pipefd[1]);
				int pid = fork();
				if(pid == 0)
				{
					// if(arr[i-1])
					// 	printf("child: arr-1: |%s|\n",arr[i-1]);
					if(i>0 && arr[i-1] && arr[i-1][ft_strlen(arr[i-1]) -1] && arr[i-1][ft_strlen(arr[i-1]) -1] == '|' && (!arr[i-1][ft_strlen(arr[i-1]) -2] || (arr[i-1][ft_strlen(arr[i-1]) -2] && arr[i-1][ft_strlen(arr[i-1]) -2] != '|')))
					{
						dup2(vars->prev_pipefd[0], 0);
						close(vars->prev_pipefd[1]);
						close(vars->prev_pipefd[0]);
					

						// char line[101];
						// int r =read(vars->next_pipefd[0], line, 100);
						// line[r] = 0;
						// write(2,line,ft_strlen(line));

						// close(vars->next_pipefd[1]);
						// close(vars->next_pipefd[0]);



						printf("in child && pipe before me arr: |%s| && arr-1: |%s|\n",arr[i],arr[i-1]);
					}
					// printf("im in child\n");
					nested_par(kobi, check, index + 1);
					// close(vars->prev_pipefd[1]);
					// close(vars->prev_pipefd[0]);
					exit(vars->ex_status);
				}
				else
				{
					if(waitpid(pid, &status, 0) != -1)
						(vars->ex_status) = WEXITSTATUS(status);

					
					// close(vars->prev_pipefd[1]);
					close(vars->prev_pipefd[0]);
				}
			}
			else
			{
				return nested_par(kobi, check, index + 1);
			}
		}
		else
		{

			if(check && i-1>0 && arr[i][0] && arr[i][0] == '|' && (!arr[i][1] || (arr[i][1] && arr[i][1] != '|')))
			{
				//end with pipe && next is subshell
				printf("in main && subshell before me and i start with pipe arr: |%s| && arr-1: |%s|\n",arr[i],arr[i-1]);

			}
			if(check && arr[i+1] && par_coount(arr[i+1]) == 1 && arr[i+1][0] == '(' && arr[i + 1][ft_strlen(arr[i + 1]) - 1] == ')'
				&& arr[i][ft_strlen(arr[i]) -1] && arr[i][ft_strlen(arr[i]) -1] == '|' &&(!arr[i][ft_strlen(arr[i]) - 2] || (arr[i][ft_strlen(arr[i]) - 2] && arr[i][ft_strlen(arr[i]) - 2] != '|')))
			{
				//end with pipe && next is subshell
				printf("in main && subshell after me and i end with pipe arr: |%s| && arr+1: |%s|\n",arr[i],arr[i+1]);
				vars->pipe = 1;
				// pipe(vars->next_pipefd);
				// vars->std_out = dup(1);
				// dup2(vars->next_pipefd[1],1);

				// close(vars->next_pipefd[1]);
				// close(vars->next_pipefd[0]);
			}
			// if(check)
			// 	printf("1       arr: %s || ex status: %d      || op: %s\n",arr[i],vars->ex_status,vars->op);
			
			// j = -1;
			// tmp = initial_split( arr[i], 1);
			// if (!tmp)
			// 	return ;
			// while (tmp[++j])
			// 	tmp[j] = ft_strtrim(tmp[j], " ");
			// parsing_commands( tmp);
			// vars->command_head = NULL;

			// printf("index: %d\n",index);
			// printf("index: %d\n",index);
			// index++;
			// printf("im currently on: %s\n", arr[i]);
			// int j = -1;
			// while(arr[++j])
			// 	printf("arr: %s\n", arr[j]);
			// printf("op: %s || iter_else_count: %d\n", vars->op,iter_else_count);
			// printf("command: %s || op: %s || it: %d || status: %d\n",arr[i],vars->op,vars->iter_else_count,vars->ex_status);

			// printf("else arr: %s\n",arr[i]);
			if (check)
			{
				// printf("cmd: %s || gg1 || i: %d || it: %d\n",arr[i],i,vars->iter_else_count);
				
				int j = -1;
				while (vars->iter_else_count > 0 && ++j < vars->iter_else_count)
				{
					if(waitpid(-1, &status, 0) != -1)
						(vars->ex_status) = WEXITSTATUS(status);
				}

				arr[i] = ft_strtrim(arr[i], " \t");

				if (arr[i][0] == '&' || (arr[i][0] == '|' && arr[i][1] && arr[i][1] == '|'))
				{
					//printf("arr[i]: %s   i: %d\n",arr[i],i);
					int s;
					char op;

					s = -1;
					if(vars->ex_status == 0 && arr[i][0] == '|')
					{
						op = '&';
						while(arr[i][++s])
						{
							if(arr[i][s+1] && arr[i][s] == op && arr[i][s+1] == op)
							{
								if (arr[i][s+2])
									s += 2;
								break;
							}
						}
					}
					else if (vars->ex_status != 0 && arr[i][0] == '&')
					{
						op = '|';
						//printf("|%s| =>>>>>>> \n", arr[i]);
						while(arr[i][++s])
						{
							if(arr[i][s+1] && arr[i][s] == op && arr[i][s+1] == op)
							{
								if (arr[i][s+2])
									s += 2;
								break;
							}
						}
						// printf("%d tt\n", s);
					}
					//  printf("before substr arr: |%s|    s: %d\n",arr[i],s);
					
					int valid = 0;
					if (s == -1)
					{
						s = 2;
						valid = 1;
					}
					// printf("%d\n", s);
					arr[i] = ft_substr(arr[i], s, -1);

					// printf("after substr arr: |%s|    s: %d\n",arr[i],s);
					if(!arr[i][0])
					{
						// i++;
						if(valid && arr[i + 1])
						{
							// printf("gg\n");
							return nested_par(&arr[i + 1], check, index +1);
						}
						else if (!valid)
						{
							while (arr[i] && ft_strtrim(arr[i], " \t")[0] != op)
								i++;
							if (arr[i])
							{
								// vars->op = operations(arr[i]);
								if(ft_strtrim(arr[i], " \t")[0] == op && ft_strlen(ft_strtrim(arr[i], " \t")) == 2)
									i++;
								return nested_par(split_par(arr[i]), check, index +1);
							}
						}
					}
				}
				
				else //if (arr[i][ft_strlen(arr[i]) - 1] == '&' || (arr[i][ft_strlen(arr[i]) - 1] == '|' && arr[i][ft_strlen(arr[i]) - 2] && arr[i][ft_strlen(arr[i]) - 2] == '|'))
				{
					// raha lfo9
				}
					// printf("gg\n");
				if (!arr[i])
				{
					// printf("cmd: %s || gg2 || i: %d || it: %d\n",arr[i],i,vars->iter_else_count);
					return 0;
				}

				// printf("total: %s || operator: %c || cmd: %s\n",vars->op, vars->op[(i - 1) * 2 +1], (*tmp)->command_args[0]);
			}
			
			if (!vars->op)
				vars->op = ft_strdup("");
			if(arr[i + 1] || (!arr[i+1]  && (arr[i][0] == '&' || arr[i][0] == '|') && index > 0))
			{
				char *tmp_op = ft_strtrim(arr[i], " \n\t");
				// if((!ft_strncmp(tmp_op, "&&", -1) || !ft_strncmp(tmp_op, "||", -1) || !ft_strncmp(tmp_op, "|", -1)) && arr[i +1])
				// {
				// 	i++;
				// 	arr[i] = ft_strtrim(arr[i], " \t\n");
				// 	if(par_coount(arr[i]) == 1 &&  arr[i][0] == '(' && arr[i][ft_strlen(arr[i]) - 1] == ')')
				// 	{
				// 		// arr[i] = ft_substr(arr[i], 1, ft_strlen(arr[i]) - 2);
				// 		return nested_par(split_par(arr[i]), check, index +1);
				// 	}
				// 	//REMIND ME LATER
				// }
				// printf("bbbbb  %s\n",arr[i]);
				vars->initial_commands = initial_split( arr[i], 1);
			}
			else
			{
				// printf("aaaaaaa  %s\n",arr[i]);
				vars->initial_commands = initial_split( arr[i], 0);
			}
			vars->iter_else_count++;
			if (!vars->initial_commands)
			{
				// printf("ll: |%s|    iter else: %d    index: %d\n",arr[i],vars->iter_else_count,index);
				return 0;
			}
			if(check)
			{

				// printf("index count: %d\n",index);
				// if(vars->iteration > 0 && im_i_in_9aws(arr[i]))
				// {
				// 	printf("im in 9aws |%s|\n",arr[i]);

				// 	// if(has pipe)
				// 	// 	pipe(gggg)
				// 	int pid = fork();
				// 	if(pid == 0)
				// 	{
				// 		printf("command count in child: %d\n",vars->command_count);
				// 		remove_spaces_in_between();
				// 		if(!parsing_commands( vars->initial_commands))
				// 			return 0;
				// 		execution_phase();
				// 		// printf("qq\n");
				// 		vars->command_head = NULL;
				// 		exit(vars->ex_status);
				// 	}
				// 	else
				// 	{
				// 		if(waitpid(pid, &status, 0) != -1)
				// 			(vars->ex_status) = WEXITSTATUS(status);
				// 	}
				// }
				// else
				// {
					// printf("comm count: %d\n",vars->command_count);
					remove_spaces_in_between();
					if(!parsing_commands( vars->initial_commands))
						return 0;
					execution_phase();
					// printf("qq\n");
					vars->command_head = NULL;
				// }
				// }
			}
		}
	}
	return (1);
}

void	split_char_init(int n[4])
{
	n[i] = -1;
	n[j] = -1;
	n[ac] = 0;
}
