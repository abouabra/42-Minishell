SRC = env_stuff.c env_linked_list_stuff.c command_linked_list_stuff.c parse_command.c split_command.c \
	split_initial.c path_stuff.c extra.c parse_command_2.c execution_phase.c \
	builtins_env.c builtins_shell.c handle_signals.c help.c parse_command3.c exec_help.c \
	split_bonus.c wildcard_bonus.c redir_command_linked_list.c

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
WWW = #-g -fsanitize=address # -Wall -Wextra -Werror

all: $(NAME)

$(NAME): ascci_art $(OSRC) src/main.c
	@make -C libft > /dev/null
	@$(CC) $(READ_LINE) $(WWW)  $(INCLUDE) $(OSRC) src/main.c  libft/libft.a -o $@
	@printf "$(RED)-------------------------------------------------------------- MiniShell Done --------------------------------------------------------------$(RESET)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c includes/minishell.h # main.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(WWW) -c $< -o $@

bonus: all

clean:
	@make -C libft clean
	@rm -rf $(OBJ_DIR)

fclean:
	@make -C libft fclean
	@rm -rf $(OBJ_DIR) $(NAME)

re:fclean all

ascci_art:
	@printf "$(GREEN)	                                                                                                                                         \n\
	                                                                                                                                         \n\
                                                                                                                                             \n\
MMMMMMMM               MMMMMMMM  iiii                      iiii     SSSSSSSSSSSSSSS hhhhhhh                                 lllllll lllllll  \n\
M:::::::M             M:::::::M i::::i                    i::::i  SS:::::::::::::::Sh:::::h                                 l:::::l l:::::l  \n\
M::::::::M           M::::::::M  iiii                      iiii  S:::::SSSSSS::::::Sh:::::h                                 l:::::l l:::::l  \n\
M:::::::::M         M:::::::::M                                  S:::::S     SSSSSSSh:::::h                                 l:::::l l:::::l  \n\
M::::::::::M       M::::::::::Miiiiiii nnnn  nnnnnnnn    iiiiiii S:::::S             h::::h hhhhh           eeeeeeeeeeee     l::::l  l::::l  \n\
M:::::::::::M     M:::::::::::Mi:::::i n:::nn::::::::nn  i:::::i S:::::S             h::::hh:::::hhh      ee::::::::::::ee   l::::l  l::::l  \n\
M:::::::M::::M   M::::M:::::::M i::::i n::::::::::::::nn  i::::i  S::::SSSS          h::::::::::::::hh   e::::::eeeee:::::ee l::::l  l::::l  \n\
M::::::M M::::M M::::M M::::::M i::::i nn:::::::::::::::n i::::i   SS::::::SSSSS     h:::::::hhh::::::h e::::::e     e:::::e l::::l  l::::l  \n\
M::::::M  M::::M::::M  M::::::M i::::i   n:::::nnnn:::::n i::::i     SSS::::::::SS   h::::::h   h::::::he:::::::eeeee::::::e l::::l  l::::l  \n\
M::::::M   M:::::::M   M::::::M i::::i   n::::n    n::::n i::::i        SSSSSS::::S  h:::::h     h:::::he:::::::::::::::::e  l::::l  l::::l  \n\
M::::::M    M:::::M    M::::::M i::::i   n::::n    n::::n i::::i             S:::::S h:::::h     h:::::he::::::eeeeeeeeeee   l::::l  l::::l  \n\
M::::::M     MMMMM     M::::::M i::::i   n::::n    n::::n i::::i             S:::::S h:::::h     h:::::he:::::::e            l::::l  l::::l  \n\
M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iSSSSSSS     S:::::S h:::::h     h:::::he::::::::e          l::::::ll::::::l \n\
M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iS::::::SSSSSS:::::S h:::::h     h:::::h e::::::::eeeeeeee  l::::::ll::::::l \n\
M::::::M               M::::::Mi::::::i  n::::n    n::::ni::::::iS:::::::::::::::SS  h:::::h     h:::::h  ee:::::::::::::e  l::::::ll::::::l \n\
MMMMMMMM               MMMMMMMMiiiiiiii  nnnnnn    nnnnnniiiiiiii SSSSSSSSSSSSSSS    hhhhhhh     hhhhhhh    eeeeeeeeeeeeee  llllllllllllllll \n\
                                                                                                                                             \n\
                                                                                                                     by: abouabra && ykhayri \n\
                                                                                                                                             \n\$(RESET)"


.PHONY: all clean fclean re bonus