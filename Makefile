# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 09:31:15 by ttas              #+#    #+#              #
#    Updated: 2025/11/05 13:42:06 by ttas             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXE = webserv

SRC =	./srcs/main.cpp


SOURCES = ${SRC}


OBJDIR = obj
OBJECTS = $(patsubst ./srcs/%, $(OBJDIR)/srcs/%, $(SOURCES:.c=.o))


FLAGS = -Wall -Werror -Wextra -Includes -std=c++98
RM = rm -f
CC = c++


$(OBJDIR)/%.o: ./%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

all:		${EXE}

${EXE}:		${OBJECTS} 
		@${CC} ${FLAGS} ${OBJECTS} -o ${EXE}
	
ac: all clean

clean:
		@${RM} ${OBJECTS}

fclean:		clean
		@${RM} ${EXE}

re:			fclean all

.PHONY: all clean fclean re ac