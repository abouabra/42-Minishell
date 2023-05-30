/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/30 19:42:03 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <unistd.h>

static void	rm_qts_help(int *num, char **arr, char *q, t_fill_info *info)
{
	char	*new;
	char	**tmp;

	*num = ft_strchr_num(arr[0], q[0]);
	if (arr[0][*num - 1] == '=')
	{
		tmp = ft_split(arr[0], '=');
		new = ft_strtrim(tmp[1], q);
		arr[0] = ft_strjoin(tmp[0], "=");
		arr[0] = ft_strjoin(arr[0], new);
	}
	else
	{
		arr[0] = ft_strtrim(arr[0], q);
		if (q[0] == '\'')
			info->quote_type = 1;
		else
			info->quote_type = 2;
	}
}

void fix_string(t_fill_info *info, char **arr)
{
    char *str = *arr;
    int len = ft_strlen(str);
    char *new_str = (char *)my_alloc(len + 1);
    if (!new_str)
        return;
    int i = 0;
    int j = 0;
    while (str[i] != '\0')
    {
		if(str[0] != '\"' && str[ft_strlen(str)-1] != '\"')
		{
			if (str[i] == '\'' && str[i + 1] && str[i + 1] == '\'')
            	i+= 2;
			// else if(str[i] == '\'')
			// 	i++;
			info->quote_type = 1;
		}
		if(str[0] != '\'' && str[ft_strlen(str)-1] != '\'')
		{
			if (str[i] == '\"' && str[i + 1] && str[i + 1] == '\"')
            	i+= 2;
			// else if(str[i] == '\"')
			// 	i++;
			info->quote_type = 2;
		}

		new_str[j] = str[i];
		j++;
        i++;
    }
    new_str[j] = '\0';
    *arr = new_str;
}

int	remove_quotes(t_fill_info *info, char **arr)
{
	int		i;
	int		num_1;
	int		num_2;
	int		sin = 0;
	int		dubl = 0;
	
	(void) vars;
	i = -1;
	while (arr[++i])
	{
		int j = -1;
		while (arr[i][++j])
		{
			if(arr[i][j] == '\'')
				sin++;	
			else if(arr[i][j] == '\"')
				dubl++;
		}
	}
	if(sin % 2 != 0 || dubl % 2 != 0)
	{
		ft_dprintf(2, "minishell: unexpected EOF while looking for matching\n");
		*vars->ex_status = 2;
		return 0;
	}
	// i = -1;
	// while (arr[++i])
	// 	fix_string(info, &arr[i]);
	i = -1;
	while (arr[++i])
	{
		if (arr[i][0] == '\'' && arr[i][1]  && arr[i][1] != '\'')
			rm_qts_help(&num_1, &arr[i], "\'", info);
		else if (arr[i][0] == '\"'  && arr[i][1]  && arr[i][1] != '\"')
			rm_qts_help(&num_2, &arr[i], "\"", info);
	}
	return 1;
}




char *gg(char *original_string,int should_expand)
{
	int i = 0;
	char *final ="";
	i = -1;
	char tmp[2];
	// if(ft_strnstr(original_string, "$?", -1))
	// 	return (original_string);
	while(original_string[++i])
	{
		if(original_string[i] == '$' && should_expand)
		{
			i++;
			if(original_string[i] == '?')
			{
				char *data = ft_itoa(*vars->ex_status);
				final = ft_strjoin(final, data);
			}
			else
			{

			int j=-1;
			while(original_string[++j + i])
			{
				if(ft_strchr(" \"'\n", original_string[j + i]))
					break;
			}
			char *id = ft_substr(original_string , i, j);
			char *data = get_env_data(id);
			if(!data)
				data = "";
			// printf("id: |%s|        data: |%s|\n", id,data);
			final = ft_strjoin(final, data);
			i += j -1;
			}
			// printf("if: %s\n", final);
		}
		else
		{
			tmp[0] = original_string[i];
			tmp[1] = '\0';
			final = ft_strjoin(final, tmp);
			// printf("else: %s\n", final);
		}
	}
	// printf("expand_env: %s\n", final);
	return (final);
}


char	*expand_env(t_fill_info *info, char *str)
{
	if(!str || !ft_strchr(str, '$'))
		return (str);
	int should_expand;
	should_expand = 1;
	if(info->quote_type == 1 || info->quote_type == 2)
		should_expand = 0;
		
	// printf("should_expand: %d    Mode: %d\n", should_expand, mode);
		
	char *tt = gg(str,should_expand);
	return (tt);
}

char	*get_herdoc_data(t_fill_info *info, char *limiter)
{
	char	*str;
	char	*total;

	limiter = expand_env(info,ft_strtrim(limiter,"\"\'"));
	limiter = ft_strjoin(limiter, "\n");
	total = "";
	while (1)
	{
		if(isatty(STDIN_FILENO))
			ft_dprintf(1, "> ");
		str = get_next_line(0);
		char *ww = expand_env(info,str);
		if (!ww || (ww && !ft_strncmp(ww, limiter, -1)))
			break ;
		// printf("str : |%s| limiter:  |%s|\n", ww,limiter);
		total = ft_strjoin(total, ww);
	}
	char *name = "/tmp/herdoc_data";
	unlink(name);
	int fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	write(fd, total, ft_strlen(total));
	close(fd);
	return (name);
}

static void	red_help(t_fill_info *info, char **commands, int *i)
{
	if (!ft_strncmp(commands[*i], ">", -1) )
	{
		check_permision(NULL, commands[*i + 1], 3);
		t_cmd_redir *redir =ft_new_redir(OUTPUT, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<", -1) )
	{
		check_permision(NULL, commands[*i + 1], 2);
		t_cmd_redir *redir =ft_new_redir(INPUT, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], ">>", -1))
	{
		check_permision(NULL, commands[*i + 1], 3);
		t_cmd_redir *redir =ft_new_redir(APPEND, commands[++(*i)]);
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<<", -1))
	{
		char *heredoc_file = get_herdoc_data(info,commands[++(*i)]);
		t_cmd_redir *redir =ft_new_redir(HEREDOC, heredoc_file);
		add_redir_in_back(&info->redir, redir);

	}
}

int count_redirections(char *command)
{
    int count = 0;
    int len = ft_strlen(command);
    int i = 0;
    int is_parsing_command = 1;

    while (i < len)
    {
        if (command[i] == '<' || command[i] == '>')
        {
            if (command[i] == '<')
            {
				if (i + 1 < len && command[i + 1] != ' ' && command[i + 1] == '<')
				{
					i++;
                    count++;
				}
				else if(i + 1 < len && command[i + 1] != ' ' && command[i + 1] != '<')
                    count++;

            }
            else if (command[i] == '>')
            {
				if ((i + 1 < len) && command[i + 1] != ' ' && command[i + 1] != '>')
				{
					i++;
                    count++;
				}
				else if(command[i + 1] != ' ' && command[i + 1] == '>')
                    count++;
            }
            is_parsing_command = 0; // Switch to parsing redirection
        }
        else if (command[i] == ' ' && !is_parsing_command)
        {
            is_parsing_command = 1; // Switch to parsing command
        }
        i++;
    }

    if (is_parsing_command && (len > 0 && command[len - 1] != ' '))
        count++; // If the command ends with a non-space character, increment count
    return count;
}



char **fix_redirections(char **commands)
{
	int i = -1;
	int count = 0;
	
	while (commands[i])
    {
        count += count_redirections(commands[i]);
        i++;
    }
	printf("count :%d\n",count);
	char **arr = my_alloc(sizeof(char *) * (count + 1));
	i = 0;
	int j = 0;
	int n;
	while(commands[i])
	{
		if(ft_strnstr(commands[i], "<<", -1) && commands[i][ft_strchr_num(commands[i], '<') + 2] && commands[i][ft_strchr_num(commands[i], '<') + 2] != ' ')
		{
				n = ft_strchr_num(commands[i], '<');
				char *tmp_1 = ft_substr(commands[i], 0, n + 2 );
				char *tmp_2 = ft_substr(commands[i], n + 2, ft_strlen(commands[i]));
				arr[j++] = tmp_1;
				arr[j] = tmp_2;
		}
		if(ft_strnstr(commands[i], ">>", -1) && commands[i][ft_strchr_num(commands[i], '>') + 2] && commands[i][ft_strchr_num(commands[i], '>') + 2] != ' ')
		{
				n = ft_strchr_num(commands[i], '>');
				char *tmp_1 = ft_substr(commands[i], 0, n + 2 );
				char *tmp_2 = ft_substr(commands[i], n + 2, ft_strlen(commands[i]));
				arr[j++] = tmp_1;
				arr[j] = tmp_2;
		}
		if(ft_strnstr(commands[i], ">", -1) && commands[i][ft_strchr_num(commands[i], '>') + 1] && commands[i][ft_strchr_num(commands[i], '>') + 1] != ' ' && commands[i][ft_strchr_num(commands[i], '>') + 1] != '>')
		{
				n = ft_strchr_num(commands[i], '>');
				char *tmp_1 = ft_substr(commands[i], 0, n + 1 );
				char *tmp_2 = ft_substr(commands[i], n + 1, ft_strlen(commands[i]));
				arr[j++] = tmp_1;
				arr[j] = tmp_2;
		}
		if(ft_strnstr(commands[i], "<", -1) && commands[i][ft_strchr_num(commands[i], '<') + 1] && commands[i][ft_strchr_num(commands[i], '<') + 1] != ' ' && commands[i][ft_strchr_num(commands[i], '>') + 1] != '<')
		{
				n = ft_strchr_num(commands[i], '<');
				char *tmp_1 = ft_substr(commands[i], 0, n + 1 );
				char *tmp_2 = ft_substr(commands[i], n + 1, ft_strlen(commands[i]));
				arr[j++] = tmp_1;
				arr[j] = tmp_2;
		}
		else
			arr[j] = commands[i];
		i++;
		j++;
	}
	arr[j] = NULL;

	i = -1;
	while(arr[++i])
		printf("arr[%d]: %s\n",i,arr[i]);
	return arr;
}

int	parse_redirections(t_fill_info *info, char ***commands)
{
	int	i;

	// *commands = fix_redirections(*commands);
	i = -1;
	while (commands[0][++i])
	{
		if(!rederiction_error(*commands, i))
			return 0;
		red_help(info, *commands, &i);
	}
	return 1;
}
