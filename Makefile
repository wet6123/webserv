NAME = webserv
CPP = c++
SRCS = $(shell find ./Sources -type f -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)
FLAG = -Wall -Wextra -Werror -std=c++98 -Iheader -g #-fsanitize=address
%.o : %.cpp
	$(CPP) $(FLAG) -c -o $@ $^

$(NAME) : $(OBJS)
	$(CPP) $(FLAG) $(OBJS) -o $@

all : $(NAME)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: re all clean fclean