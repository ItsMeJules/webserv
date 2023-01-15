NAME		=	webserv
CC			=	g++
CFLAGS		=	-fsanitize=address -g -std=c++98
RM			=	rm -rf
SRC_DIR 	= 	srcs/
SRC		 	=	$(notdir $(shell find $(SRC_DIR) -type f -name "*.cpp"))
OBJ_DIR		=	obj
OBJ 		= 	$(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))
INC_DIR		=	headers
INC			=	$(shell find $(INC_DIR) -type f -name "*.hpp")
IFLAGS 		=	-I $(INC_DIR)
vpath			%.cpp $(shell find $(SRC_DIR) -type d)
.SUFFIXES: 		.cpp .o .hpp

_YELLOW		=	\033[38;5;184m
_GREEN		=	\033[38;5;46m
_RESET		=	\033[0m
_INFO		=	[$(_YELLOW)INFO$(_RESET)]
_SUCCESS	=	[$(_GREEN)SUCCESS$(_RESET)]
_CLEAR		=	\033[2K\c

all				:	init $(NAME)
					@ echo "$(_SUCCESS) Compilation done"

bonus			:	all

init			:
					@ $(shell mkdir -p $(OBJ_DIR))

$(NAME)			:	$(OBJ) $(INC)
					@ echo "$(_INFO) Initialize $(NAME)"
				 	@ $(CC) $(CFLAGS) $(IFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o	:	%.cpp
					@ echo "\t$(_YELLOW)Compiling$(_RESET) $*.cpp\r\c"
					@ $(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
					@ echo "$(_CLEAR)"

clean			:
					@ echo "$(_INFO) Deleted object files and directories"
					@ $(RM) $(OBJ_DIR)
					@ echo "$(_SUCCESS) Working directory clean"

fclean			:	clean
					@ $(RM) $(NAME)

re				: 	fclean all

norminette		:
					@ ~/.norminette/norminette.rb $(SRC_DIR)
					@ ~/.norminette/norminette.rb $(INC_DIR)

.PHONY: 		all fclean clean re
