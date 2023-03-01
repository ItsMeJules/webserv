#	Names
NAME		=	webserv

#	Compilation
CXX			=	c++
CXXFLAG		=	-std=c++98 -Wall -Werror -Wextra -fsanitize=address -g
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

#	Color
_YELLOW		=	\033[38;5;184m
_GREEN		=	\033[38;5;46m
_RESET		=	\033[0m
_INFO		=	[$(_YELLOW)INFO$(_RESET)]
_SUCCESS	=	[$(_GREEN)SUCCESS$(_RESET)]
_CLEAR		=	\033[2K\c

#	Rules
#		Defaults:
all				:	init $(NAME)
					@ echo "$(_SUCCESS) Compilation done"

container_init	:
					mkdir -p container/volume
					mkdir -p container/html
					mkdir -p container/conf

					cp -r srcs container/volume
					cp -r headers container/volume

					cp -r html container
					cp -r conf container

					cp Makefile container/volume

#		Bonus:
bonus			:	all

#		Init:
init			:
					@ $(shell mkdir -p $(OBJ_DIR))

#		Compile:
$(NAME)			:	$(OBJ) $(INC)
					@ echo "$(_INFO) Initialize $(NAME)"
				 	@ $(CXX) $(CXXFLAG) $(IFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o	:	%.cpp
					@ echo "\t$(_YELLOW)Compiling$(_RESET) $*.cpp"
					@ $(CXX) $(CXXFLAG) $(IFLAGS) -c $< -o $@
					@ echo "$(_CLEAR)"

#		Clean:
clean			:
					@ echo "$(_INFO) Deleted object files and directories"
					@ $(RM) $(OBJ_DIR)
					@ echo "$(_SUCCESS) Working directory clean"

clean_container	:
					@ echo "$(_INFO) Deleting container files"
					@ sudo $(RM) container/volume
					@ sudo $(RM) container/conf
					@ sudo $(RM) container/html

#		FClean:
fclean			:	clean
					@ $(RM) $(NAME)

#		Re:
re				: 	fclean all

#		Norminette:
norminette		:
					@ ~/.norminette/norminette.rb $(SRC_DIR)
					@ ~/.norminette/norminette.rb $(INC_DIR)

#		Phony:
.PHONY: 		all fclean clean re
