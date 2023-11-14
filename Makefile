# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 12:47:35 by tstahlhu          #+#    #+#              #
#    Updated: 2023/11/14 16:35:44 by digallar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRSCR		= ./src/

DIRINCL		= ./includes/

INCL		= minishell libft

INCLS		= $(addprefix ${DIRINCL}, $(addsuffix .h, ${INCL}))

DIRLIBFT	= ./libft/

SRC			= builtins error_exit init main

SRCS		= $(addprefix ${DIRSCR}, $(addsuffix .c, ${SRC}))

OBJS		= ${SRCS:.c=.o}

NAME		= minishell

NAMELIBFT	= libft.a

CC			= cc

RM			= rm -f

CFLAGS		= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME): 	$(OBJS) ${INCLS} ${NAMELIBFT}
				cc ${CFLAGS} ${OBJS} -I${DIRINCL} -ledit ${NAMELIBFT} -o ${NAME}

${NAMELIBFT}:	$(shell find ${DIRLIBFT} -type f \( -iname \*.c -o -iname \*.h \))
				cd ${DIRLIBFT} && ${MAKE} bonus && cp -v ${NAMELIBFT} ../${NAMELIBFT}

.c.o:		 
			${CC} ${CFLAGS} -c -I${DIRINCL} $< -o $@

clean:
			${RM} $(OBJS)
			cd ${DIRLIBFT} && ${MAKE} clean

fclean:	clean
			${RM} $(NAME)
			${RM} $(NAMELIBFT)
			cd ${DIRLIBFT} && ${MAKE} fclean

re: fclean all

.PHONY:		all clean fclean re
