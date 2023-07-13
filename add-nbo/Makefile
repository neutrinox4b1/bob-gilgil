CXX = g++
NAME = add-nbo
SRCS = add-nbo.cpp
OBJS = add-nbo.o

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.c
	$(CXX) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) $(NAME)