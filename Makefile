# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 12:47:35 by tstahlhu          #+#    #+#              #
#    Updated: 2024/01/21 17:48:23 by tstahlhu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRSCR		= ./src/

DIRINCL		= ./includes/

INCL		= minishell libft

INCLS		= $(addprefix ${DIRINCL}, $(addsuffix .h, ${INCL}))

DIRLIBFT	= ./libft/

SRC			= builtins error_exit init main memory parser/parser commands_executor \
				utils fs_split env_var redirections pipe parser/command_splitter \
				parser/command_splitter2 parser/vars_expander parser/vars_expander2 \
				parser/command_creator parser/redirections_builder parser/quotes_remover \
				parser/argv_builder2 parser/argv_allocator utils2 parser/vars_expander3 \
				parser/argv_builder parser/redirections_builder2 parser/redirections_builder3 \
				redirections_multiple heredoc builtins2

SRCS		= $(addprefix ${DIRSCR}, $(addsuffix .c, ${SRC}))

OBJS		= ${SRCS:.c=.o}

NAME		= minishell

NAMELIBFT	= libft.a

CC			= cc

RM			= rm -f

CFLAGS		= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME): 	$(OBJS) ${INCLS} ${NAMELIBFT}
				cc ${CFLAGS} ${OBJS} -I${DIRINCL} -l readline ${NAMELIBFT} -o ${NAME}

${NAMELIBFT}:	$(shell find ${DIRLIBFT} -type f \( -iname \*.c -o -iname \*.h \))
				cd ${DIRLIBFT} && ${MAKE} bonus && cp -v ${NAMELIBFT} ../${NAMELIBFT}

.c.o:
			${CC} ${CFLAGS} -c -g -I${DIRINCL} $< -o $@

clean:
ifneq ("$(wildcard $(OBJS))", "")
			${RM} $(OBJS)
			cd ${DIRLIBFT} && ${MAKE} clean
endif

fclean:	clean
ifneq ("$(wildcard $(NAME) $(NAMELIBFT))", "")
			${RM} $(NAME)
			${RM} $(NAMELIBFT)
			cd ${DIRLIBFT} && ${MAKE} fclean
endif

re: fclean all

.PHONY:		all clean fclean re
