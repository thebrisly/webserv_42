SRCS = $(wildcard sources/*.cpp)

NAME = ./server

OBJS := $(SRCS:%.cpp=%.o)

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CPPFLAGS) $(OBJS) -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
