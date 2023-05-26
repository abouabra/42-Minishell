SRC = env_stuff.c env_linked_list_stuff.c command_linked_list_stuff.c parse_command.c split_command.c \
	split_initial.c path_stuff.c extra.c parse_command_2.c execution_phase.c \
	builtins_env.c builtins_shell.c handle_signals.c help.c parse_command3.c exec_help.c \
	split_bonus.c wildcard_bonus.c

SRC_DIR = src/
OBJ_DIR = obj/

OSRC = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

NAME= minishell
INCLUDE = -I. -I libft -I includes

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

READ_LINE = -lreadline -L ${HOME}/brew/homebrew/opt/readline/lib -I${HOME}/brew/homebrew/opt/readline/include
WWW = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OSRC) src/main.c
	@make -C libft > /dev/null
	@$(CC) $(READ_LINE) $(WWW)  $(INCLUDE) $(OSRC) src/main.c  libft/libft.a -o $@
	@printf "$(RED)--------MINI SHELL DONE--------$(RESET)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c includes/minishell.h # main.c
	@mkdir -p $(OBJ_DIR)
	@printf "hana kan compili f$(GREEN) $< $(RESET) \n"
	@$(CC) -Wall -Wextra -Werror -c $< -o $@
bonus: all

clean:
	@make -C libft clean
	@rm -rf $(OBJ_DIR)

fclean:
	@make -C libft fclean
	@rm -rf $(OBJ_DIR) $(NAME)

re:fclean all

tst: all
	@cd tester && ./test.sh && cd ..

.PHONY: all clean fclean re bonus