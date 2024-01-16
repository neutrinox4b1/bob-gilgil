CC = gcc
NAME = airodump
SRCS = main.c airodump.c
OBJS = $(SRCS:.c=.o)
LIBS = -lpcap

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re	
