/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:46:47 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/16 17:13:15 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> /* readline */
# include <readline/readline.h> /*readline*/
# include <readline/history.h> /*readline*/
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft.h"
# include "ft_printf.h"

# define PROMPT "Minishell>$ "

typedef enum s_cmdtype {
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	EXEC
}	t_cmdtype;

typedef struct s_command
{
	t_cmdtype	cmd;
	char		**argv;
	char		*i_redirect;
	char		*o_redirect;
	char		*a_redirect;
}			t_command;

typedef struct s_data 
{
	t_list	*free_list;
	t_list	*commands;
	char	*user_input;
}			t_data;

/* init.c */
void    *init_zero();

/* builtins.c */
void    builtins(t_data *data);

/* error_exit.c */
void    free_exit(t_data *data);

/* memory.c */
void	*safe_malloc(t_data *data, size_t size);

/* parser.c */
void	parse_input(t_data *data);

#endif /* MINISHELL_H */
