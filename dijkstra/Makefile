NAME   =  dij
SRCS   =  edge.cpp \
		  main.cpp \
		  vertex.cpp \
		  vertexQueue.cpp

OBJS   =  $(SRCS:.cpp=.o)
CC     =  g++
CFLAGS =  -c -Wall -Werror -Wextra -std=c++11

.PHONY: clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

$(OBJS):
	$(CC) $(CFLAGS) $(SRCS)

clean:
	/bin/rm -f $(OBJS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
