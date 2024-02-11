# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 12:47:35 by tstahlhu          #+#    #+#              #
#    Updated: 2024/02/11 13:42:07 by digallar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRSCR		= ./src/

DIRINCL		= ./includes/

INCL		= minishell libft

INCLS		= $(addprefix ${DIRINCL}, $(addsuffix .h, ${INCL}))

DIRLIBFT	= ./libft/

SRC			= 	main init error_exit memory signals \
				parser/parser parser/command_splitter parser/command_splitter2 \
				parser/vars_expander parser/vars_expander2 parser/vars_expander3 \
				parser/command_creator parser/redirections_builder \
				parser/quotes_remover parser/argv_builder2 parser/argv_allocator  \
				parser/argv_builder parser/redirections_builder2 parser/redirections_builder3 \
				executor/commands_executor executor/pipe \
				executor/redirections executor/redirections_multiple executor/heredoc \
				builtins/builtin_cd builtins/builtin_exit builtins/builtin_export \
				builtins/builtins builtins/env_var  \
				utils/utils utils/utils2 utils/fs_split utils/fs_itoa

SRCS		= $(addprefix ${DIRSCR}, $(addsuffix .c, ${SRC}))

OBJS		= ${SRCS:.c=.o}

NAME		= minishell

NAMELIBFT	= libft.a

CC			= cc

RM			= rm -f

CFLAGS		= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME): 	$(OBJS) ${INCLS} ${NAMELIBFT}
				cc ${CFLAGS} ${OBJS} -I${DIRINCL} -l readline ${NAMELIBFT} -o ${NAME} -L/usr/local/opt/readline/lib

${NAMELIBFT}:	$(shell find ${DIRLIBFT} -type f \( -iname \*.c -o -iname \*.h \))
				cd ${DIRLIBFT} && ${MAKE} bonus && cp -v ${NAMELIBFT} ../${NAMELIBFT}

.c.o:
			${CC} ${CFLAGS} -c -g -I${DIRINCL} $< -o $@ -I/usr/local/opt/readline/include

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
