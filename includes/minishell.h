/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:46:47 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/19 20:13:43 by digallar         ###   ########.fr       */
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

typedef enum s_parse_status {
	WAITING_FOR_CHAR,
	FOUND_SQOT_WFC,
	FOUND_SQOT,
	FOUND_DQOT_WFC,
	FOUND_DQOT,
	WAITING_FOR_SPACE,
	WAITING_FOR_PIPE

}	t_parse_status;

typedef struct s_command
{
	t_cmdtype	cmd;
	char		*cmd_input;
	char		**argv;
	char		*i_redirect;
	char		*o_redirect;
	char		*a_redirect;
	int			exit_code;
	t_list		*free_list;
}			t_command;

typedef struct s_data 
{
	t_list	*free_list;
	t_list	*commands;
	int		nb_cmds; /*this is good to know when piping and forking*/
	char	*user_input;
	char	**env; /*these are the environment variables (will be inherited)*/
	char	**shvar;	/*these are the shell variables (only present in this shell)*/
}			t_data;

/* init.c */
void    *init_data(char **env);
int		init_env(t_data *data, char **env);

/* builtins.c */
void    check_builtins(t_data *data, t_command *command);

/* commands_executor.c */
int single_command(t_data *data, t_command *command);

/* error_exit.c */
void	free_data(t_data *data);
void    free_exit(t_data *data);
void    free_command(t_command *command);


/* memory.c */
void	*safe_malloc(t_data *data, size_t size);
void	*free_arr_str(char **str);
void	*command_safe_malloc(t_command *command, size_t size);

/* parser.c */
void	parse_input(t_data *data);

/* env_var.c */
char   **add_var(t_data *data, char **var, char *new_var);
char    **del_var(t_data *data, char **var, char *del_var);

/* utils.c */
char	*fs_strdup(t_data *data, char *s);
int	ft_strvcmp(const char *s1, const char *s2);

/* sf_split.c */
char	**fs_split(t_data *data, char const *s, char c);

#endif /* MINISHELL_H */
