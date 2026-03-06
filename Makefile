NAME = codexion
SRCS = main.c dongle.c queue.c thread_routines.c time.c 
OBJS = ${SRCS:.c=.o}
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all : ${NAME}
	
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f ${OBJS}
	
fclean:	clean
		rm -f ${NAME}
re: fclean all

.PHONY :all clean fclean re