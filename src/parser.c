/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2024/01/13 16:45:20 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\n' || c == '\t' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r');
}

void	skip_spaces(t_command *command, int *i)
{
	while(ft_isspace(command->cmd_input[*i]))
	{
		(*i)++;
	}
}

void remove_quotes(char *arg)
{
	int	i;
	t_parse_status status;

	i = -1;
	status = WAITING_FOR_QOT;
	while (arg[++i])
	{
		if (arg[i] == '\"')
		{
			if (status == WAITING_FOR_QOT) {
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
				status = FOUND_DQOT;
			} else if (status == FOUND_DQOT) {
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			} else if (status == FOUND_SQOT)
				continue;
		}
		else if (arg[i] == '\'')
		{
			if (status == WAITING_FOR_QOT) {
				status = FOUND_SQOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			} else if (status == FOUND_SQOT) {
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			} else if (status == FOUND_DQOT)
				continue;
		}
	}
}

/*

Returns the difference from the length of the $VAR with the length of the value it holds.

Have to put content inside double quotes as only $ can be interpreted as a special sign, other symbols like > cannot.

*/
int expand_var(t_command *command, char **env, int start, int end, int inside_quotes)
{
	char *env_var;
	char *env_var_value;
	char *tmp_cmd;
	char old_size;

	tmp_cmd = command->cmd_input;
	old_size = ft_strlen(command->cmd_input);
	env_var = fs_substr(command->cmd_input, start, (end - start) + 1, command);
	env_var_value = find_var(env, &env_var[1]);
	if (!env_var_value)
	{
		env_var_value = command_safe_malloc(command,1);
		env_var_value[0] = 0;
	}
	if (!inside_quotes)
	{
		env_var_value = fs_strjoin("\"", env_var_value, command);
		env_var_value = fs_strjoin(env_var_value, "\"", command);
	}
	tmp_cmd = fs_strjoin(fs_substr(command->cmd_input, 0, start, command), env_var_value, command);
	command->cmd_input = fs_strjoin(tmp_cmd, fs_substr(command->cmd_input, end + 1, ft_strlen(command->cmd_input) - end, command), command);
	printf("Trying to replace [%s] with [%s] resulted in [%s]\n", env_var, env_var_value, command->cmd_input);
	return (ft_strlen(command->cmd_input) - old_size);
}

/* Returns the difference from the length of the $? with the length of the value it holds */
int expand_exit_status(t_command *command, int es, int start, int end, int inside_quotes)
{
	char *env_var_value;
	char *tmp_cmd;
	char old_size;

	tmp_cmd = command->cmd_input;
	old_size = ft_strlen(command->cmd_input);
	env_var_value = ft_itoa(es);
	if (!inside_quotes)
	{
		env_var_value = fs_strjoin("\"", env_var_value, command);
		env_var_value = fs_strjoin(env_var_value, "\"", command);
	}
	tmp_cmd = fs_strjoin(fs_substr(command->cmd_input, 0, start, command), env_var_value, command);
	command->cmd_input = fs_strjoin(tmp_cmd, fs_substr(command->cmd_input, end + 1, ft_strlen(command->cmd_input) - end, command), command);
	printf("Trying to replace [$?] with [%s] resulted in [%s]\n", env_var_value, command->cmd_input);
	return (ft_strlen(command->cmd_input) - old_size);
}

int	create_limiter(t_command *command, int start, int end)
{
	int	i;
	int old_size;

	i = start;
	old_size = ft_strlen(command->cmd_input);
	while (1)
	{
		if (command->cmd_input[i] != '<' && command->cmd_input[i] != '>' && !ft_isspace(command->cmd_input[i]))
			break;
		i++;
	}
	command->limiter = fs_substr(command->cmd_input, i, end - i, command);

	command->cmd_input = fs_strjoin(fs_substr(command->cmd_input, 0, start, command), fs_substr(command->cmd_input, end, ft_strlen(command->cmd_input) - end, command), command);
	command->found_limiter = 0;
	remove_quotes(command->limiter);
	printf("Limiter = [%s]\n", command->limiter);
	return (ft_strlen(command->cmd_input) - old_size);
}

int	create_redirection(t_command *command, int start, int end)
{
	int	i;
	int type;
	int old_size;

	if (command->found_limiter == 1)
		return create_limiter(command, start, end);
	i = start;
	type = -1;
	old_size = ft_strlen(command->cmd_input);
	while (1)
	{
		if (command->cmd_input[i] == '<')
			type = 0;
		else if(command->cmd_input[i] == '>')
			type = 1;
		else if (!ft_isspace(command->cmd_input[i]))
			break;
		i++;
	}
	if (type == 0)
	{
		command->stdin = fs_substr(command->cmd_input, i, end - i, command);
		remove_quotes(command->stdin);
		printf("Redirection input = [%s]\n", command->stdin);
	}
	else if (type == 1)
	{
		command->stdout = fs_substr(command->cmd_input, i, end - i, command);
		remove_quotes(command->stdout);
		printf("Redirection output = [%s]\n", command->stdout);
	}
	command->cmd_input = fs_strjoin(fs_substr(command->cmd_input, 0, start, command), fs_substr(command->cmd_input, end, ft_strlen(command->cmd_input) - end, command), command);
	return (ft_strlen(command->cmd_input) - old_size);
}

int build_redirections(t_command *command)
{
	int	i;
	int start_i;
	t_parse_status status;

	i = -1;
	start_i = 0;
	status = WAITING_FOR_RED;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\'')
		{
			if (status == WAITING_FOR_RED)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_RED;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == FOUND_IRED)
				status = FOUND_SQOT_WFS;
			else if (status == FOUND_ORED)
				status = FOUND_SQOT_WFS;
			else if (status == FOUND_SQOT_WFS)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				status = FOUND_SQOT_WFS;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT_WFS;
		}
		else if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_RED)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_RED;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_IRED)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_ORED)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_DQOT_WFS)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFS;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_DQOT_WFS;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_RED)
				continue;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_IRED)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_ORED)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_DQOT_WFS)
				continue;
			else if (status == FOUND_SQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				status = WAITING_FOR_RED;
				i += create_redirection(command, start_i, i);
			}
		}
		else if (command->cmd_input[i] == '>')
		{
			if (status == WAITING_FOR_RED)
			{
				status = FOUND_ORED;
				start_i = i;
				command->append_mode = 0;
			}
			else if (status == FOUND_DQOT)
				continue;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_IRED)
				return (printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == FOUND_ORED)
			{
				command->append_mode = 1;
				status = WAITING_FOR_CHAR;
			}
			else if (status == FOUND_DQOT_WFS)
				continue;
			else if (status == FOUND_SQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				return (printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == WAITING_FOR_SPACE)
			{
				status = FOUND_ORED;
				i += create_redirection(command, start_i, i);
			}
		}
		else if (command->cmd_input[i] == '<')
		{
			if (status == WAITING_FOR_RED)
			{
				status = FOUND_IRED;
				start_i = i;
			}
			else if (status == FOUND_DQOT)
				continue;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_ORED)
				return (printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == FOUND_IRED)
			{
				command->found_limiter = 1;
				status = WAITING_FOR_CHAR;
			}
			else if (status == FOUND_DQOT_WFS)
				continue;
			else if (status == FOUND_SQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				return (printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == WAITING_FOR_SPACE)
			{
				status = FOUND_IRED;
				i += create_redirection(command, start_i, i);
			}
		}
		else
		{
			if (status == WAITING_FOR_RED)
				continue;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_ORED)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_IRED)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFS)
				continue;
			else if (status == FOUND_SQOT_WFS)
				continue;
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == WAITING_FOR_SPACE)
				continue;
		}
	}
	if (status == FOUND_ORED)
		return (-2);
	else if (status == FOUND_IRED)
		return (-2);
	else if (status == WAITING_FOR_CHAR)
		return (-2);
	else if (status == WAITING_FOR_SPACE)
		create_redirection(command, start_i, i);
	return (0);
}

void expand_vars(t_command *command, char **env, int es)
{
	int	i;
	int start_i;
	t_parse_status status;

	i = -1;
	start_i = 0;
	status = WAITING_FOR_DOLLAR;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\'')
		{
			if (status == WAITING_FOR_DOLLAR)
				status = FOUND_SQOT_WFD;
			else if (status == FOUND_SQOT_WFD)
				status = WAITING_FOR_DOLLAR;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = FOUND_SQOT_WFD;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = FOUND_SQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFD)
				continue;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_DOLLAR)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = FOUND_DQOT_WFD;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = FOUND_DQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFD)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = WAITING_FOR_DOLLAR;
			}
		}
		else if (command->cmd_input[i] == '?')
		{
			if (status == WAITING_FOR_DOLLAR)
				continue;
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_exit_status(command, es, start_i, i, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == FOUND_DQOT_WFD)
				continue;
			else if (status == FOUND_DQOT)
			{
				i += expand_exit_status(command, es, start_i, i, 1);
				status = FOUND_DQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_DOLLAR)
				continue;
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFD)
				continue;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else if (command->cmd_input[i] == '$')
		{
			if (status == WAITING_FOR_DOLLAR)
			{
				start_i = i;
				status = WAITING_FOR_CHAR;
			}
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = WAITING_FOR_CHAR;
				start_i = i;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = WAITING_FOR_CHAR;
				start_i = i;
			}
			else if (status == FOUND_DQOT_WFD)
			{
				start_i = i;
				status = FOUND_DQOT;
			}
			else if (status == FOUND_DQOT)
				start_i = i;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
				start_i = i;
			}
		}
		else if (!ft_isalnum(command->cmd_input[i]) && command->cmd_input[i] != '_')
		{
			if (status == WAITING_FOR_DOLLAR)
				continue;
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, env, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFD)
				continue;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, env, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else
		{
			if (status == WAITING_FOR_DOLLAR)
				continue;
			else if (status == FOUND_SQOT_WFD)
				continue;
			else if (status == WAITING_FOR_SPACE)
				continue;
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFD)
				continue;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_DQOT_WFS)
				continue;
		}
	}
	if (status == WAITING_FOR_SPACE)
	{
		i += expand_var(command, env, start_i, i - 1, 0);
	}
}

void build_argv(t_command *command)
{
	int	i;
	int pos;
	int start_i;
	t_parse_status status;

	i = -1;
	pos = 0;
	start_i = 0;
	status = WAITING_FOR_CHAR;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_CHAR)
			{
				start_i = i;
				status = FOUND_DQOT_WFC;
			}
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_SPACE;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_DQOT;
		}
		else if (command->cmd_input[i] == '\'')
		{
			if (status == WAITING_FOR_CHAR)
			{
				start_i = i;
				status = FOUND_SQOT_WFC;
			}
			else if (status == FOUND_SQOT_WFC)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				status = WAITING_FOR_CHAR;
				command->argv[pos] = fs_substr(command->cmd_input, start_i, i - start_i, command);
				remove_quotes(command->argv[pos]);
				printf("Argument = [%s]\n", command->argv[pos]);
				start_i = i + 1;
				pos++;
			}
		}
		else
		{
			if (status == WAITING_FOR_CHAR)
			{
				start_i = i;
				status = WAITING_FOR_SPACE;
			}
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
		}
	}
	if (status == WAITING_FOR_SPACE)
	{
		command->argv[pos] = fs_substr(command->cmd_input, start_i, i - start_i, command);
		remove_quotes(command->argv[pos]);
		printf("Argument = [%s]\n", command->argv[pos]);
	}
}

void alloc_argv(t_command *command)
{
	int	i;
	int size;
	t_parse_status status;

	i = -1;
	size = 0;
	status = WAITING_FOR_CHAR;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_SPACE;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_DQOT;
		}
		else if (command->cmd_input[i] == '\'')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_SQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				status = WAITING_FOR_CHAR;
				size++;
			}
		}
		else
		{
			if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
		}
	}
	if (status == WAITING_FOR_SPACE)
		size++;
	printf("Command has %i arguments.\n", size);
	command->argv = command_safe_malloc(command, sizeof(command->argv) * (size + 1));
	command->argv[size] = 0;
}

void	add_type(t_command *command)
{
	if (!strncmp(command->argv[0], "echo", ft_strlen(command->argv[0])))
		command->cmd = ECHO;
	else if (!strncmp(command->argv[0], "cd ", ft_strlen(command->argv[0])))
		command->cmd = CD;
	else if (!strncmp(command->argv[0], "pwd", ft_strlen(command->argv[0])))
		command->cmd = PWD;
	else if (!strncmp(command->argv[0], "env", ft_strlen(command->argv[0])))
		command->cmd = ENV;
	else if (!strncmp(command->argv[0], "exit", ft_strlen(command->argv[0])))
		command->cmd = EXIT;
	else if (!strncmp(command->argv[0], "export", ft_strlen(command->argv[0])))
		command->cmd = EXPORT;
	else if (!strncmp(command->argv[0], "unset", ft_strlen(command->argv[0])))
		command->cmd = UNSET;
	else
		command->cmd = EXEC;
}

int	create_command(t_data *data, int start, int end)
{
	int code;

	t_command *command = safe_malloc(data, sizeof(t_command));
	if (!data->commands)
		data->commands = ft_lstnew(command);
	else
		ft_lstadd_back(&data->commands, ft_lstnew(command));
	command->cmd_input = fs_substr(data->user_input, start, end - start, command);
	command->free_list = data->free_list;
	command->stdin = 0;
	command->stdout = 0;
	command->limiter = 0;
	command->found_limiter = 0;
	command->append_mode = -1;
	printf("Command input = [%s]\n", command->cmd_input);
	expand_vars(command, data->env, data->exit_status);
	code = build_redirections(command);
	if (code != 0)
		return (code); //FREE_COMMAND HERE
	alloc_argv(command);
	build_argv(command);
	add_type(command);
	printf("Command append_mode = [%d]\n", command->append_mode);
	printf("Command stdin = [%s]\n", command->stdin);
	printf("Command stdout = [%s]\n", command->stdout);
	printf("Command limiter = [%s]\n", command->limiter);
	return (0);
}

//WHEN 258/-1/-2 is returned have to check outside to not handle any already interpreted command, it should just print error
int	split_commands(t_data *data)
{
	int	i;
	t_parse_status status;
	int start_i;
	int	code;

	i = -1;
	start_i = 0;
	status = WAITING_FOR_CHAR;
	while (data->user_input[++i])
	{
		if (data->user_input[i] == '\"')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_PIPE;
			else if (status == WAITING_FOR_PIPE)
				status = FOUND_DQOT;
		}
		else if (data->user_input[i] == '\'')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_SQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
				status = FOUND_SQOT;
		}
		else if (data->user_input[i] == '|')
		{
			if (status == WAITING_FOR_CHAR)
			{
				printf("-minishell: syntax error near unexpected token `|'\n");
				return (258);
			}
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
			{
				code = create_command(data, start_i, i);
				if (code != 0)
				{
					if (code == -2)
						printf("-minishell: syntax error near unexpected token `|'\n");
					return (code);
				}
				start_i = i + 1;
				data->nb_cmds++;
				status = WAITING_FOR_CHAR;
			}
		}
		else if (ft_isspace(data->user_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
				continue;
		}
		else
		{
			if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
		}
	}
	if (status == WAITING_FOR_PIPE)
	{
		code = create_command(data, start_i, i);
		if (code != 0)
		{
			if (code == -2)
				printf("-minishell: syntax error near unexpected token `newline'\n");
			return (code);
		}
		data->nb_cmds++;
	}
	return (0);
}

void	init_commands(t_data *data)
{
	data->nb_cmds = 0;
	data->commands = 0;

}

void	parse_input(t_data *data)
{
	init_commands(data);
	split_commands(data);

	//code do split user_input into cmd_input
	//code to break command_input into tokens
	/* one block for each command */
	//first command
	// data->nb_cmds = 1;
	// t_command *command1 = safe_malloc(data, sizeof(t_command));

	// command1->argv = fs_split(data, data->user_input, ' ');
	/* one block for each command */
	//first command
	//	t_command *command1 = safe_malloc(data, sizeof(t_command));
	//	command1->cmd = ECHO;
	//	command1->argv = safe_malloc(data, 8 * 4); /* 2 is the number of arguments for the command */
	//		command1->argv[0] = "echo";
	//		command1->argv[1] = "abc";
	//		command1->argv[2] = "$USER";
	//		command1->argv[3] = NULL;
	//	command1->i_redirect = "/folde/input.txt"; /* set to 0 to use stdin */
	//	command1->o_redirect = "/folde/output.txt"; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//	command1->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */

	//second command
	//t_command *command2 = safe_malloc(data, sizeof(t_command));
	//	command2->cmd = CD;
	//	command2->argv = safe_malloc(data, 8 * 3); /* 2 is the number of arguments for the command */
	//		command2->argv[0] = "cd";
	//		command2->argv[1] = "/home/tstahlhu/42";
	//		command2->argv[2] = NULL;
	//	command2->i_redirect = 0; /* set to 0 to use stdin */
	//	command2->o_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//	command2->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//for the first command
	// data->commands = ft_lstnew(command1);

	//for every additional command
	//ft_lstadd_back(&data->commands, ft_lstnew(command2));
	//ft_lstadd_back(&data->commands, ft_lstnew(command3));
}
