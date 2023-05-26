/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:01 by abouabra          #+#    #+#             */
/*   Updated: 2023/05/26 23:52:45 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct t_fill_info
{
	char				*command_path;
	char				**command_args;

	int					quote_type;
	int					is_valid_command;

	int					is_input;
	char				*input_file;
	int					is_output;
	char				*output_file;
	int					is_append;
	char				*append_file;
	int					is_herdoc;
	char				*herdoc_limiter;
	char				*herdoc_data;
}						t_fill_info;

//sort < input.txt | uniq -l  -a >>
// unique_sorted_output.txt > outfile.txt << HEREDOC

typedef struct t_env
{
	char				*env_id;
	char				*env_data;
	struct t_env		*next;
}						t_env;

typedef struct t_chars
{
	char				**arr;
	char				*strings[2];
}						t_chars;

typedef struct t_command
{
	char				*command_path;
	char				**command_args;

	int					quote_type;
	int					is_valid_command;

	int					is_input;
	char				*input_file;
	int					is_output;
	char				*output_file;
	int					is_append;
	char				*append_file;
	int					is_herdoc;
	char				*herdoc_data;
	char				*herdoc_limiter;
	struct t_command	*next;
}						t_command;

typedef struct t_args
{
	char				**initial_commands;
	char				**ev;
	int					command_count;
	int					is_quote;
	int					prev_pipefd[2];
	int					next_pipefd[2];
	t_command			*command_head;
	t_env				*env_head;
	struct termios		new_term;
	struct termios		old_term;
	int					*is_running;
	int					*ex_status;
	int					*is_interupted;
}						t_args;


void					prepare_commands(char **commands);
void					parsing_initial_split(char **commands);
char					**get_path();
char					*get_command_path(char **path, char *command);

//command linked list stuff
t_command				*ft_last_command(t_command *lst);
void					add_command_in_back(t_command **head, t_command *node);
void					add_command_in_front(t_command **head, t_command *node);
int						ft_command_list_size(t_command **head);
t_command				*ft_new_command(t_fill_info *info);

//env linked list stuff
t_env					*ft_last_env(t_env *lst);
void					add_env_in_back(t_env **head, t_env *node);
void					add_env_in_front(t_env **head, t_env *node);
t_env					*ft_new_env_node(char *env_id, char *env_data);
void					ft_node_remove_if(t_env **head, char *env_id);
int						ft_env_list_size(t_env **head);
void					the_search_env(t_env **search, char **args);

//parsing commands;
void					parse_commands(char *line);
char					**initial_split(char *s, int sw);
char					**split_command(char *s);

void					set_env();
char					*get_env_data(char *id);
void					remove_quotes(t_fill_info *info,
							char **arr);
char					*get_herdoc_data(char *limiter);
void					parse_redirections(t_fill_info *info, char **commands);
int						count_args(char **commands);
char					**make_new_args(char **commands);
int						check_permision(char *command_path, char *name,
							int arg);
void					parsing_commands(char **commands);
char					**split_par(char *s);
int						has_char(char *s, char c);
void					nested_par(char **arr);
char					*wildcard(char *av);
void					rederiction_error(char **commands, int i);
int						checker(char **commands, int i);
void					dollar_active(int n[4], char *strings[4], char **args);
void					split_char_init(int n[4]);
void					doub_sin_skip(int *sin, int *doub, char *s, int i);
void					handle_child(t_command *tmp, int fd,
							int i);

//executing phase

void					execution_phase();
void					execute_built_in(t_command *command);
void					execute(t_command *tmp, int i);
char					**convert_env_to_arr(t_env *head);

//builtins
int						is_built_in(char *name);
void					echo(t_command *command);
void					cd(t_command *command);
void					pwd(void);
void					env_export(t_command *command);
void					unset(char *env_id);
void					env();
void					my_exit(t_command *command);

void					print_command(t_command *command);

//signals
void					init_termio();
void					handle_signals(int sig);

int						is_second_arg_number(char **str);
void					fd_handler(int i);
char	*is_arg_number(char *str);

extern t_args			*vars;


enum					e_nums
{
	i,
	j,
	k,
	ph_len,
	si,
	dou
};
enum					e_quotes
{
	sin,
	doub,
	in_word
};
enum					e_arrays
{
	arr,
	args,
	path
};
enum					e_split
{
	w = 2,
	ac
};
enum					e_wild
{
	tmp,
	final,
	len = 3,
	valid
};
enum					e_expand
{
	data = 1,
	news,
	str
};
enum					e_e
{
	is_running,
	ex_status,
	is_interupted
};
#endif
