SRC = env_stuff.c env_linked_list_stuff.c command_linked_list_stuff.c parse_command.c split_command.c \
	split_initial.c path_stuff.c extra.c parse_command_2.c execution_phase.c \
	builtins_env.c builtins_shell.c handle_signals.c help.c parse_command3.c exec_help.c\
	split_bonus.c wildcard_bonus.c

OSRC=$(SRC:.c=.o)

NAME= minishell
INCLUDE = -I. -I libft

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

READ_LINE = -lreadline -L ${HOME}/.brew/opt/readline/lib -I${HOME}/.brew/opt/readline/include
WWW = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OSRC) main.c
	@make -C libft
	@$(CC) main.c $(READ_LINE) $(WWW)  $(INCLUDE) $(OSRC) libft/libft.a -o $@
	@printf "$(RED)--------MINI SHELL DONE--------$(RESET)\n"

%.o : %.c minishell.h
#    @printf "hana kan compili f$(GREEN) $< $(RESET) \n"
	@$(CC) -Wall -Wextra -Werror -c $<

clean:
	@make -C libft clean
	@rm -rf *.o

fclean:
	@make -C libft fclean
	@rm -rf *.o $(NAME)

re:fclean all

.PHONY: all clean fclean re