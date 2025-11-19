# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 09:31:15 by ttas              #+#    #+#              #
#    Updated: 2025/11/19 11:12:21 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXE = webserv

SRC =	./srcs/main.cpp \
		./srcs/ServerConfig.cpp


SOURCES = ${SRC}


OBJDIR = obj
OBJECTS = $(patsubst ./srcs/%, $(OBJDIR)/srcs/%, $(SOURCES:.cpp=.o))


FLAGS = -Wall -Werror -Wextra -Includes -std=c++98
RM = rm -f
CC = c++


$(OBJDIR)/%.o: ./%.cpp
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