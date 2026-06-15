NAME = codexion
SRCS = coder/main.c coder/dongle.c coder/queue.c coder/coders.c coder/monitoring.c coder/time.c coder/validation.c
OBJS = ${SRCS:.c=.o}
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -std=c89

all : ${NAME}
	
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f ${OBJS}
	
fclean:	clean
		rm -f ${NAME}
re: fclean all

.PHONY :all clean fclean re