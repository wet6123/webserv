NAME       = WebServer
CXX        = c++
CXXFLAG    = -Wall -Wextra -Werror -std=c++98

# ===============================================
# Source Directories
# ===============================================
SRC_DIRS   = client/src \
             request/src \
             socket/src \
             response/src \
             common \
             server/src \

# ===============================================
# Source Files
# ===============================================
SRCS       = $(foreach dir,$(SRC_DIRS),$(shell find $(dir) -name "*.cpp" -and -not -name "main.cpp"))

# ===============================================
# Header Directories
# ===============================================
HEAD_DIRS  = client/inc \
             request/inc \
             socket/inc \
             response/inc \
             common \
             server/inc \

MAIN_SRC   = main.cpp

MAIN_OBJ   = $(MAIN_SRC:.cpp=.o)
SRC_CUR    = $(addsuffix , $(SRCS))
OBJ        = $(SRC_CUR:.cpp=.o) $(MAIN_OBJ)
HEAD       = $(foreach dir,$(HEAD_DIRS),$(shell find $(dir) -name "*.hpp"))

# ===============================================
# Color Codes
# ===============================================
Black      = \033[0;30m
Red        = \033[1;31m
Green      = \033[0;32m
Yellow     = \033[0;33m
Blue       = \033[0;34m
Purple     = \033[4;35m
Cyan       = \033[1;36m
White      = \033[0;37m
Gray       = \033[0;90m
Pink       = \033[0;35m
DEF_COLOR  = \033[0;39m

# ===============================================
# Targets
# ===============================================
all: $(NAME)

$(NAME): $(OBJ) $(HEAD)
	@$(CXX) $(CXXFLAG) $(OBJ) -o $(NAME)
	@echo "\033[1;32mCompilation complete. $(NAME) has been created.\033[0m"

.cpp.o: $(HEAD)
	@$(CXX) $(CXXFLAG) -c -o $@ $<
	@echo "\033[1;34mCompiling $<...\033[0m"

clean:
	@rm -rf $(OBJ)
	@printf "$(LF) $(Cyan)Cleaning...\n$(DEF_COLOR)\b"

fclean: clean
	@rm -rf $(NAME)
	@printf "$(LF) $(Red)Cleaning...\n"

re:
	@$(MAKE) fclean
	@sleep 0.5
	@clear
	@printf "$(DEF_COLOR)$(Purple)>>REBUILD<<$(DEF_COLOR)\n"
	@$(MAKE) all

.PHONY: all clean fclean re