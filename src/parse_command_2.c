/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:27:14 by abouabra          #+#    #+#             */
/*   Updated: 2023/06/01 22:10:34 by abouabra         ###   ########.fr       */
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
		// arr[0] = ft_strtrim(arr[0], q);
		if (q[0] == '\'')
			info->quote_type = 1;
		else
			info->quote_type = 2;
	}
}

/*


echo '"ls"'
"ls"

remove_quotes

str;
while (*str)
{
	if (*str == '"' || *str == '\'')
	{
		c = *str;
		ft_memmove(str, str+1, ft_strlen(str + 1));
		str = ft_strchr(str, c);
		ft_memmove(str, str+1, ft_strlen(str + 1));
	}
	else
		str++;
}

*/

void fix_string(t_fill_info *info, char *str)
{
    (void)info;
	char c;

	if(str[0] && str[1] && !str[2] && str[0] == str[1])
		return;
	if(ft_strchr(str, '=') || ft_strchr(str, '$'))
		return;
    char *dest = str;
    char *src = str;
	(void) info;
    while (*src)
    {
        if (*src == '"' || *src == '\'')
        {
            c = *src;
			if(c == '\'')
				info->quote_type = 1;
			else if(c == '\"')
				info->quote_type = 2;
            src++;

            while (*src && *src != c)
            {
                *dest = *src;
                dest++;
                src++;
            }

            if (*src == c)
                src++;
        }
        else
        {
            *dest = *src;
            dest++;
            src++;
        }
    }
    *dest = '\0';
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
	i = -1;
	while (arr[++i])
		fix_string(info, arr[i]);

	// printf("limiter: %s     qt: %d\n", arr[1], info->quote_type);
	
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
	return (final);
}


char	*expand_env(t_fill_info *info, char *str)
{
	if(!str || !ft_strchr(str, '$'))
		return (str);
	int should_expand;
	should_expand = 1;
	// printf("expand env: %s\n", str);
	// char *tmp = ft_strchr(str, '$');
	// if(tmp && tmp[-1] && (tmp[-1] != '\'' || tmp[-1] != '"'))
	// 	info->quote_type = 0;
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

	// printf("limiter: %s     qt: %d\n", limiter, info->quote_type);
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

void	red_help(t_fill_info *info, char **commands, int *i)
{
	char **arr;
	char *file_name;
	if(ft_strchr(commands[*i + 1], '$'))
	{
		arr = my_alloc(sizeof(char *) * 2);
		arr[0] = ft_strdup(commands[*i + 1]);;
		arr[1] = NULL;
		arr = expand_variables(info, arr);
		int k = -1;
		while (arr[++k])
			fix_string(info, arr[k]);
		file_name = ft_strtrim(*arr," ");
	}
	else
		file_name = commands[*i + 1];
	if (!ft_strncmp(commands[*i], ">", -1) )
	{
		check_permision(NULL, file_name, 3);
		t_cmd_redir *redir =ft_new_redir(OUTPUT, file_name);
		(*i)++;
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], "<", -1) )
	{
		check_permision(NULL, file_name, 2);
		t_cmd_redir *redir =ft_new_redir(INPUT, file_name);
		(*i)++;
		add_redir_in_back(&info->redir, redir);
	}
	if (!ft_strncmp(commands[*i], ">>", -1))
	{
		check_permision(NULL, file_name, 3);
		t_cmd_redir *redir =ft_new_redir(APPEND, file_name);
		(*i)++;
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
