NAME	= RPN
SRCS	= main.cpp RPN.cpp
OBJS	= $(SRCS:.cpp=.o)
DEP	= $(OBJS:.o=.d)
CC	= c++
CPPFLAGS= -Wall -Werror -Wextra -std=c++98 -MMD -MP#-g
RM	= rm -f

all:	$(NAME)

$(NAME):$(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

-include $(DEP)

%.o:%.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@

clean:
	$(RM) $(OBJS) $(DEP)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
