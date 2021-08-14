.PHONY: all clean fclean re

# Name of file
NAME		=	philo_bonus

# Name directory
PATH_INC	=	includes
PATH_SRC	=	srcs
PATH_OBJ	=	objs

# List of sources
SRCS_FILES	=	activity.c init_simulation.c listener.c log.c monitor.c philosopher.c time.c utils.c
SRCS		=	$(addprefix $(PATH_SRC)/, $(SRCS_FILES)) main.c
INC_FILES	=	$(shell find $(PATH_INC) -type f | cut -d/ -f2- )

OBJS		=	$(addprefix $(PATH_OBJ)/, $(SRCS:.c=.o))
INCS		=	$(addprefix $(PATH_INC)/, $(INC_FILES))

# Commands of compilation
COMP		=	clang
COMP_FLAG	=	-Wall -Werror -Wextra 
COMP_ADD	=	-I$(PATH_INC)
COMP_DEB	=	-g3 -fsanitize=address

# Others Command
RM			=	/bin/rm
# Color Code and template code
_YELLOW		=	\033[38;5;184m
_GREEN		=	\033[38;5;46m
_RESET		=	\033[0m
_INFO		=	[$(_YELLOW)INFO$(_RESET)]
_SUCCESS	=	[$(_GREEN)SUCCESS$(_RESET)]

# Functions
all:	init $(NAME)
	@ echo "$(_SUCCESS) Compilation done"

init:
	@ mkdir -p $(PATH_OBJ)/$(PATH_SRC)

$(NAME): $(OBJS) $(INCS)
	@ $(COMP) $(COMP_FLAG) $(COMP_ADD) -o $(NAME) $(OBJS)

$(PATH_OBJ)/%.o : %.c  $(INCS)
	@ ($(COMP) $(COMP_FLAG) $(COMP_ADD) -c $< -o $@ )
	@ echo "$(_INFO) Compilation of $*"

$(PATH_OBJ)/%.o : $(PATH_SRC)/%.c  $(INCS)
	@ ($(COMP) $(COMP_FLAG) $(COMP_ADD) -c $< -o $@ )
	@ echo "$(_INFO) Compilation of $*"

clean:
	@ $(RM) -rf $(PATH_OBJ)
	@ echo "$(_INFO) Deleted files and directory"

fclean: clean
	@ $(RM) -rf $(NAME)

re: fclean all