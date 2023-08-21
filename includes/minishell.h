/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:30:01 by abouabra          #+#    #+#             */
/*   Updated: 2023/08/21 15:27:51 by ykhayri          ###   ########.fr       */
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
# define SUB 10

typedef struct t_env
{
	char				*env_id;
	char				*env_data;
	int					not_declared;
	struct t_env		*next;
}						t_env;

typedef struct t_chars
{
	char				**arr;
	char				*strings[2];
}						t_chars;

typedef struct t_cmd_redir
{
	int					type;
	char				*file;
	struct t_cmd_redir	*next;
}						t_cmd_redir;

enum e_redir{
	INPUT,
	OUTPUT,
	APPEND,
	HEREDOC
};

typedef struct t_fill_info
{
	char				*command_path;
	char				**command_args;

	int					quote_type;
	int					is_valid_command;

	t_cmd_redir			*redir;
}	t_fill_info;

typedef struct t_command
{
	char				*command_path;
	char				**command_args;
	int					quote_type;
	int					is_valid_command;
	t_cmd_redir			*redir;
	t_fill_info			*info;
	struct t_command	*next;
}						t_command;

typedef struct t_args
{
	char				**initial_commands;
	char				**ev;
	int					command_count;
	int					is_quote;
	int					*pid;
	int					prev_pipefd[2];
	int					next_pipefd[2];
	t_command			*command_head;
	t_env				*env_head;
	struct termios		new_term;
	struct termios		old_term;
	char				*op;
	int					is_running;
	int					is_interrupted;
	int					interrupted_mode;
	int					ex_status;
	int					heredocs_fd;
	int					iter_else_count;
	int					iteration;
	int					pipe;
}						t_args;

int						ft_strrevcmp(char *s1, char *s2);
void					prepare_commands(char **commands);
void					parsing_initial_split(char **commands);
char					**get_path(void);
char					*get_command_path(char **path, char *command);
int						skip_opertor(char **arr, int n[SUB], int check,
							char *s);
t_command				*ft_last_command(t_command *lst);
void					add_command_in_back(t_command **head, t_command *node);
void					add_command_in_front(t_command **head, t_command *node);
int						ft_command_list_size(t_command **head);
t_command				*ft_new_command(t_fill_info *info);
int						par_coount(char *s);
t_env					*ft_last_env(t_env *lst);
void					add_env_in_back(t_env **head, t_env *node);
void					add_env_in_front(t_env **head, t_env *node);
t_env					*ft_new_env_node(char *env_id, char *env_data);
void					ft_node_remove_if(t_env **head, char *env_id);
int						ft_env_list_size(t_env **head);
void					the_search_env(t_env **search, char **args);
void					wait_pid(int n[SUB], char *op);
void					parse_commands(char *line);
char					**initial_split(char *s, int sw);
char					**split_command(char *s);
char					**extended_splitting(char **phrases);
void					set_env(void);
char					*get_env_data(char *id);
int						remove_quotes(t_fill_info *info, char **arr);
char					*get_herdoc_data(t_fill_info *info, char *limiter);
int						count_args(char **commands);
char					**make_new_args(char **commands);
int						check_permision(char *command_path, char *name,
							int arg);
int						subshell_op(int n[SUB], int check, char **arr);
int						parsing_commands(char **commands);
char					**split_par(char *s);
int						has_char(char *s, char c);
int						nested_par(char **arr, int check, int index);
char					*wildcard(char *av);
int						rederiction_error(char **commands, int i);
int						checker(char **commands, int i);
void					dollar_active(t_fill_info *info, int n[4],
							char *strings[4], char **args);
void					split_char_init(int n[4]);
void					doub_sin_skip(int *sin, int *doub, char *s, int i);
void					handle_child(t_command *tmp, int i);
void					remove_spaces_in_between(void);
char					*operations(char *s);
int						ft_strrevcmp(char *s1, char *s2);
int						not_subshell_skip(int n[SUB], char **arr, int check);
int						subshell_errors(char **kobi, int check);
void					execution_phase(void);
void					execute_built_in(t_command *command);
void					execute( t_command **tmp, int *index);
char					**convert_env_to_arr(t_env *head);
void					initial_split_init(int n[6]);
int						is_built_in(char *name);
int						built_in_should_execute_in_main(t_command *tmp);
void					echo(t_command *command);
void					cd(t_command *command);
void					pwd(void);
void					env_export(t_command *command);
void					unset(t_command *command);
void					env(void);
void					my_exit(t_command *command);
int						analyze_args(char **arg);
void					print_command(t_command *command);

void					init_termio(void);
void					handle_signals(int sig);
void					not_subshell_pieps(int n[SUB], char **arr, int check);
char					*are_two_args_number(char **str);
void					fd_handler(int i);
char					*is_arg_number(char *str);
void					sub_pipe(int n[SUB], char **arr, char **kobi,
							int check);
char					**split_arg(char *arg);
void					do_redirections(t_cmd_redir *head);
t_cmd_redir				*ft_last_redir(t_cmd_redir *lst);
void					add_redir_in_back(t_cmd_redir **head,
							t_cmd_redir *node);
t_cmd_redir				*ft_new_redir(int type, char *file);
char					**remove_empty_args(char **commands);
int						does_redirection_exist(char *str);
void					red_help(t_fill_info *info, char **commands, int *i);
void					fix_string(t_fill_info *info, char *dest, char *src);
char					**expand_variables(t_fill_info *info, char **args);
int						test_ambiguous(t_fill_info *in, char **arg);
void					start_waiting(t_command *tmp, int *status);
void					start_execution(t_command *tmp);
void					handle_signals(int signum);
void					init_termio(void);
int						subshell_errors2_help(char **op_err, int y);
void					subshell_errors2_help_quotes(int quote[2], char c);
void					exp_var_quote(t_fill_info *info, char **args, int n[4],
							char *strs[4]);
void					set_quotes_types(t_fill_info *info, char *str);

extern t_args			*g_vars;

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

enum e_tt
{
	system_defined,
	user_defined,
	no_value
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

enum					e_nested
{
	stat = 1,
	par_count,
	nest_return,
	ind,
	subshell_op_ret,
	subshell_err,
	skip_op,
	success,
	s
};

#endif
