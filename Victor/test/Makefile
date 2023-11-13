# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Werror -Wextra #-std=c++98

# Name of the executable
EXEC = my_program

# List of source files
SRCS = $(wildcard *.cpp)

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(EXEC)

# Rule to link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -fsanitize=address

# Rule for object files from cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(EXEC)

re:
	make clean
	make all
