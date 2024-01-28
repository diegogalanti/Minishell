/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:46:47 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/28 16:05:32 by digallar         ###   ########.fr       */
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

typedef enum s_cmdtype
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	EXEC,
	NOT_FOUND,
	NONE
}	t_cmdtype;

typedef enum s_parse_status
{
	WFC,
	FOUND_SQOT_WFC,
	FOUND_SQOT_WFS,
	FOUND_SQOT_IRED,
	FOUND_SQOT_ORED,
	FOUND_DQOT_IRED,
	FOUND_DQOT_IARED,
	FOUND_DQOT_ORED,
	FOUND_DQOT_OARED,
	FOUND_DQOT_WFS,
	FOUND_SQOT_WFD,
	FOUND_SQOT,
	FOUND_DQOT_WFC,
	FOUND_DQOT_WFD,
	FOUND_DQOT,
	WAITING_FOR_SPACE,
	WAITING_FOR_PIPE,
	WAITING_FOR_RED,
	FOUND_IRED,
	FOUND_ORED,
	FOUND_IARED,
	FOUND_OARED,
	WAITING_FOR_DOLLAR,
	WAITING_FOR_QOT,
	ABORT
}	t_parse_status;

typedef struct s_command
{
	t_cmdtype	cmd;
	char		*cmd_input;
	char		**argv;
	int			exit_code;
	char		*stdin;
	char		*stdout;
	int			append_mode;
	int			fd_in;
	int			fd_out;
	char		*limiter;
	int			found_limiter;
	int			exit_status;
	char		**env;
	t_list		*free_list;
}			t_command;

typedef struct s_data
{
	t_list	*free_list;
	t_list	*commands;
	int		nb_cmds; /*this is good to know when piping and forking*/
	char	*user_input;
	char	**env; /*these are the environment variables (will be inherited)*/
	char	**shvar;/*shell variables (only present in this shell)*/
	int		**pipe;
	int		*pid;
	int		stdin_cpy;
	int		stdout_cpy;
	int		exit_status;
}			t_data;

/* init.c */
void			*init_data(char **env);
int				init_env(t_data *data, char **env);

/* memory.c */
void			*safe_malloc(t_data *data, size_t size);
void			*free_arr_str(char **str);
void			*command_safe_malloc(t_command *command, size_t size);
int				close_all_fd(t_data *data);
void			close_fd(int *fd);

/* commands_executor.c */
char			*find_path(t_data *data, char *cmd);
int				single_command(t_data *data, t_command *command);
void			execute_command(t_data *data, t_command *command);
void			execute(t_data *data);

/* builtins.c & builtins2.c*/
void        	print_export(char **env);
int	            check_export_var(char *str);
void	        builtin_export(t_data *data, t_command *command);
void	        builtin_unset(t_data *data, t_command *command);
void	        builtin_env(t_data *data, t_command *command);
void			check_builtins(t_data *data, t_command *command, int i);

/* pipe.c */
int				creat_pipe(t_data *data);
int				pipe_commands(t_data *data);
void			child_process(t_data *data, t_command *command, int i);
void			get_child_exit_status(t_data *data, int child_exit_status);

/* error_exit.c */
void			free_data(t_data *data);
void			free_exit(t_data *data);
void			free_command(t_command *command);
void			exit_child(t_data *data, int exit_status);

/* parser.c */
void			parse_input(t_data *data);
int				check_commented(t_data *data);

/* env_var.c */
//char   **add_var(t_data *data, char **var, char *new_var);
char			**del_var(t_data *data, char **var, char *del_var);
char			**add_mod_var(t_data *data, char **var, char *new_var);
int				is_var(char **car, char *s);
char			*find_var(char **var, char *s);

/* redirections.c */
int				set_redirections(t_command *c);
int				close_redirections(t_command *cmd);
int				check_redirections(t_data *data, int (*f)(t_command *));
int				here_doc(t_command *command);
int				redirect(t_command *command, t_data *data);
void			undirect(t_command *command, t_data *data);

/* redirections_multiple.c */
int				find_mult_redir(char **argv);
void			trunc_mult_redir(char **argv);

/* utils.c */
char			*fs_strdup(t_data *data, char *s);
int				ft_strvcmp(const char *s1, const char *s2);
char			*fs_strjoin(char const *s1, char const *s2, t_command *command);
char			*ff_strjoin(char const *s1, char const *s2, t_data *data);

/* utils2.c */
int				ft_isspace(int c);
char			*fs_substr(char const *s, unsigned int start,
					size_t len, t_command *command);

/* fs_split.c */
char			**fs_split(t_data *data, char const *s, char c);

/* command_splitter.c */
void			split_commands(t_data *data);

/* command_splitter2.c */
t_parse_status	check_dquote_cs(t_parse_status current);
t_parse_status	check_squote_cs(t_parse_status current);

/* vars_expander.c */
void			expand_vars(t_command *cmd);

/* vars_expander2.c */
t_parse_status	check_squote_ve(t_parse_status current, int *i,
					int start_i, t_command *cmd);
t_parse_status	check_dquote_ve(t_parse_status current, int *i,
					int start_i, t_command *cmd);
t_parse_status	check_question_ve(t_parse_status current, int *i,
					int start_i, t_command *cmd);
t_parse_status	check_space_ve(t_parse_status current, int *i,
					int start_i, t_command *cmd);

/* vars_expander3.c */
int				expand_var(t_command *command, int start, int end, int iq);
int				expand_exit_status(t_command *comm, int start, int end, int iq);

/* redirections_builder.c */
int				build_redirections(t_command *command, int next_c);

/* redirections_builder2.c */
t_parse_status	check_space_rb(t_parse_status current, int start_i,
					int *i, t_command *command);
t_parse_status	check_quotesother_rb(char c, t_parse_status status);

/* redictetions_builder3.c */
int				create_redirection(t_command *cmd, int start, int end);
void			print_error_pipe(void);
void			print_error_nl(void);

/* command_creator.c */
void			create_command(t_data *data, int start, int end, int next_c);

/* argv_allocator.c */
void			alloc_argv(t_command *command);

/* argv_builder.c */
void			build_argv(t_command *command);

/* argv_builder2.c */
t_parse_status	check_nonspace_ab(t_parse_status current, int *start_i,
					int i, char c);

/* quotes_remover.c */
void			remove_quotes(char *arg);

#endif /* MINISHELL_H */
