# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/06 10:14:05 by tstahlhu          #+#    #+#              #
#    Updated: 2023/11/14 16:23:21 by digallar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
DEPFLAGS = -MP -MD
CFLAGS = -Wall -Werror -Wextra 

CFILES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
ft_printf.c ft_print_arguments.c
OBJ = $(patsubst %.c,%.o,$(CFILES))
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

BONUS_CFILES = ft_lstnew.c ft_lstlast.c ft_lstsize.c ft_lstadd_back.c \
ft_lstadd_front.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c
BONUS_OBJ = $(patsubst %.c,%.o, $(BONUS_CFILES))
BONUS_DEPFILES = $(patsubst %.c,%.d, $(BONUS_CFILES))


all: $(NAME)

$(NAME): $(OBJ)
	ar -crs $(NAME) $(OBJ)

OBJ: $(CFILES)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $<

bonus: $(NAME) $(BONUS_OBJ)
	ar -crs $(NAME) $(BONUS_OBJ)

BONUS_OBJ: $(BONUS_CFILES)
	$(CC) $(CFLAGS) $(BONUS_DEPFLAGS) -c $<

clean:
	rm -f $(DEPFILES) $(BONUS_DEPFILES) $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME) 

re:	fclean all

.PHONY: all clean fclean re
