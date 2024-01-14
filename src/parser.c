/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2024/01/14 09:42:43 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\n' || c == '\t' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r');
}

void	skip_spaces(t_command *command, int *i)
{
	while (ft_isspace(command->cmd_input[*i]))
	{
		(*i)++;
	}
}

void	remove_quotes(char *arg)
{
	int				i;
	t_parse_status	status;

	i = -1;
	status = WAITING_FOR_QOT;
	while (arg[++i])
	{
		if (arg[i] == '\"')
		{
			if (status == WAITING_FOR_QOT)
			{
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
				status = FOUND_DQOT;
			}
			else if (status == FOUND_DQOT)
			{
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_SQOT)
				continue ;
		}
		else if (arg[i] == '\'')
		{
			if (status == WAITING_FOR_QOT)
			{
				status = FOUND_SQOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_SQOT)
			{
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_DQOT)
				continue ;
		}
	}
}

int	expand_var(t_command *command, int start, int end, int iq)
{
	char	*env_var;
	char	*env_var_value;
	char	*tmp_cmd;
	char	old_size;

	tmp_cmd = command->cmd_input;
	old_size = ft_strlen(command->cmd_input);
	env_var = fs_substr(command->cmd_input, start, (end - start) + 1, command);
	env_var_value = find_var(command->env, &env_var[1]);
	if (!env_var_value)
	{
		env_var_value = command_safe_malloc(command, 1);
		env_var_value[0] = 0;
	}
	if (!iq)
	{
		env_var_value = fs_strjoin("\"", env_var_value, command);
		env_var_value = fs_strjoin(env_var_value, "\"", command);
	}
	tmp_cmd = fs_strjoin(fs_substr(command->cmd_input, 0, start, command),
			env_var_value, command);
	command->cmd_input = fs_strjoin(tmp_cmd, fs_substr(command->cmd_input,
				end + 1, ft_strlen(command->cmd_input) - end, command),
			command);
	return (ft_strlen(command->cmd_input) - old_size);
}

int	expand_exit_status(t_command *comm, int start, int end, int iq)
{
	char	*env_var_value;
	char	*tmp_cmd;
	char	old_size;

	tmp_cmd = comm->cmd_input;
	old_size = ft_strlen(comm->cmd_input);
	env_var_value = ft_itoa(comm->exit_status);
	if (!iq)
	{
		env_var_value = fs_strjoin("\"", env_var_value, comm);
		env_var_value = fs_strjoin(env_var_value, "\"", comm);
	}
	tmp_cmd = fs_strjoin(fs_substr(comm->cmd_input, 0, start, comm),
			env_var_value, comm);
	comm->cmd_input = fs_strjoin(tmp_cmd, fs_substr(comm->cmd_input,
				end + 1, ft_strlen(comm->cmd_input) - end, comm), comm);
	return (ft_strlen(comm->cmd_input) - old_size);
}

int	create_limiter(t_command *cmd, int start, int end)
{
	int	i;
	int	old_size;

	i = start;
	old_size = ft_strlen(cmd->cmd_input);
	while (1)
	{
		if (cmd->cmd_input[i] != '<' && cmd->cmd_input[i] != '>'
			&& !ft_isspace(cmd->cmd_input[i]))
			break ;
		i++;
	}
	cmd->limiter = fs_substr(cmd->cmd_input, i, end - i, cmd);
	cmd->cmd_input = fs_strjoin(fs_substr(cmd->cmd_input, 0, start, cmd),
			fs_substr(cmd->cmd_input, end,
				ft_strlen(cmd->cmd_input) - end, cmd), cmd);
	cmd->found_limiter = 0;
	remove_quotes(cmd->limiter);
	ft_printf("Limiter = [%s]\n", cmd->limiter);
	return (ft_strlen(cmd->cmd_input) - old_size);
}

int	get_redirection_type(t_command *command, int start, int *i)
{
	int	type;

	*i = start;
	type = -1;
	while (1)
	{
		if (command->cmd_input[*i] == '<')
			type = 0;
		else if (command->cmd_input[*i] == '>')
			type = 1;
		else if (!ft_isspace(command->cmd_input[*i]))
			break ;
		(*i)++;
	}
	return (type);
}

int	create_redirection(t_command *cmd, int start, int end)
{
	int	i;
	int	type;
	int	old_size;

	if (cmd->found_limiter == 1)
		return (create_limiter(cmd, start, end));
	type = get_redirection_type(cmd, start, &i);
	old_size = ft_strlen(cmd->cmd_input);
	if (type == 0)
	{
		cmd->stdin = fs_substr(cmd->cmd_input, i, end - i, cmd);
		remove_quotes(cmd->stdin);
		ft_printf("Redirection input = [%s]\n", cmd->stdin);
	}
	else if (type == 1)
	{
		cmd->stdout = fs_substr(cmd->cmd_input, i, end - i, cmd);
		remove_quotes(cmd->stdout);
		ft_printf("Redirection output = [%s]\n", cmd->stdout);
	}
	cmd->cmd_input = fs_strjoin(fs_substr(cmd->cmd_input, 0, start, cmd),
			fs_substr(cmd->cmd_input, end,
				ft_strlen(cmd->cmd_input) - end, cmd), cmd);
	return (ft_strlen(cmd->cmd_input) - old_size);
}

int	build_redirections(t_command *command)
{
	int				i;
	int				start_i;
	t_parse_status	status;

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
				continue ;
			else if (status == FOUND_IRED)
				status = FOUND_SQOT_WFS;
			else if (status == FOUND_ORED)
				status = FOUND_SQOT_WFS;
			else if (status == FOUND_SQOT_WFS)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFS)
				continue ;
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
				continue ;
			else if (status == FOUND_IRED)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_ORED)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_DQOT_WFS)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT_WFS)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFS;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_DQOT_WFS;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_RED)
				continue ;
			else if (status == FOUND_DQOT)
				continue ;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_IRED)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_ORED)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_DQOT_WFS)
				continue ;
			else if (status == FOUND_SQOT_WFS)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				continue ;
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
				continue ;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_IRED)
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == FOUND_ORED)
			{
				command->append_mode = 1;
				status = WAITING_FOR_CHAR;
			}
			else if (status == FOUND_DQOT_WFS)
				continue ;
			else if (status == FOUND_SQOT_WFS)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
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
				continue ;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_ORED)
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == FOUND_IRED)
			{
				command->found_limiter = 1;
				status = WAITING_FOR_CHAR;
			}
			else if (status == FOUND_DQOT_WFS)
				continue ;
			else if (status == FOUND_SQOT_WFS)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), -1);
			else if (status == WAITING_FOR_SPACE)
			{
				status = FOUND_IRED;
				i += create_redirection(command, start_i, i);
			}
		}
		else
		{
			if (status == WAITING_FOR_RED)
				continue ;
			else if (status == FOUND_DQOT)
				continue ;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_ORED)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_IRED)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFS)
				continue ;
			else if (status == FOUND_SQOT_WFS)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == WAITING_FOR_SPACE)
				continue ;
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

void	expand_vars(t_command *command)
{
	int				i;
	int				start_i;
	t_parse_status	status;

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
				i += expand_var(command, start_i, i - 1, 0);
				status = FOUND_SQOT_WFD;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = FOUND_SQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFD)
				continue ;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_DOLLAR)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_SQOT_WFD)
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = FOUND_DQOT_WFD;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = FOUND_DQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFD)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, start_i, i - 1, 1);
				status = WAITING_FOR_DOLLAR;
			}
		}
		else if (command->cmd_input[i] == '?')
		{
			if (status == WAITING_FOR_DOLLAR)
				continue ;
			else if (status == FOUND_SQOT_WFD)
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_exit_status(command, start_i, i, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == FOUND_DQOT_WFD)
				continue ;
			else if (status == FOUND_DQOT)
			{
				i += expand_exit_status(command, start_i, i, 1);
				status = FOUND_DQOT_WFD;
			}
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_DOLLAR)
				continue ;
			else if (status == FOUND_SQOT_WFD)
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFD)
				continue ;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, start_i, i - 1, 1);
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
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = WAITING_FOR_CHAR;
				start_i = i;
			}
			else if (status == WAITING_FOR_CHAR)
			{
				i += expand_var(command, start_i, i - 1, 0);
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
				i += expand_var(command, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
				start_i = i;
			}
		}
		else if (!ft_isalnum(command->cmd_input[i]) && command->cmd_input[i] != '_')
		{
			if (status == WAITING_FOR_DOLLAR)
				continue ;
			else if (status == FOUND_SQOT_WFD)
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				i += expand_var(command, start_i, i - 1, 0);
				status = WAITING_FOR_DOLLAR;
			}
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_DOLLAR;
			else if (status == FOUND_DQOT_WFD)
				continue ;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFD;
			else if (status == FOUND_DQOT_WFS)
			{
				i += expand_var(command, start_i, i - 1, 1);
				status = FOUND_DQOT_WFD;
			}
		}
		else
		{
			if (status == WAITING_FOR_DOLLAR)
				continue ;
			else if (status == FOUND_SQOT_WFD)
				continue ;
			else if (status == WAITING_FOR_SPACE)
				continue ;
			else if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFD)
				continue ;
			else if (status == FOUND_DQOT)
				status = FOUND_DQOT_WFS;
			else if (status == FOUND_DQOT_WFS)
				continue ;
		}
	}
	if (status == WAITING_FOR_SPACE)
	{
		i += expand_var(command, start_i, i - 1, 0);
	}
}

void	build_argv(t_command *command)
{
	int				i;
	int				pos;
	int				start_i;
	t_parse_status	status;

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
				continue ;
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
				continue ;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue ;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue ;
			else if (status == WAITING_FOR_SPACE)
			{
				status = WAITING_FOR_CHAR;
				command->argv[pos] = fs_substr(command->cmd_input, start_i, i - start_i, command);
				remove_quotes(command->argv[pos]);
				ft_printf("Argument = [%s]\n", command->argv[pos]);
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
		ft_printf("Argument = [%s]\n", command->argv[pos]);
	}
}

void	alloc_argv(t_command *command)
{
	int				i;
	int				size;
	t_parse_status	status;

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
				continue ;
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
				continue ;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue ;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue ;
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
	ft_printf("Command has %i arguments.\n", size);
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
	int			code;
	t_command	*command ;

	command = safe_malloc(data, sizeof(*command));
	if (!data->commands)
		data->commands = ft_lstnew(command);
	else
		ft_lstadd_back(&data->commands, ft_lstnew(command));
	command->env = data->env;
	command->free_list = data->free_list;
	command->cmd_input = fs_substr(data->user_input, start, end - start, command);
	command->stdin = 0;
	command->stdout = 0;
	command->limiter = 0;
	command->exit_status = data->exit_status;
	command->found_limiter = 0;
	command->append_mode = -1;
	ft_printf("Command input = [%s]\n", command->cmd_input);
	expand_vars(command);
	code = build_redirections(command);
	if (code != 0)
		return (code);
	alloc_argv(command);
	build_argv(command);
	add_type(command);
	ft_printf("Command append_mode = [%d]\n", command->append_mode);
	ft_printf("Command stdin = [%s]\n", command->stdin);
	ft_printf("Command stdout = [%s]\n", command->stdout);
	ft_printf("Command limiter = [%s]\n", command->limiter);
	return (0);
}

//WHEN 258/-1/-2 is returned have to check outside to not handle any already interpreted command, it should just print error
int	split_commands(t_data *data)
{
	int				i;
	t_parse_status	status;
	int				start_i;
	int				code;

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
				continue ;
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
				continue ;
			else if (status == WAITING_FOR_PIPE)
				status = FOUND_SQOT;
		}
		else if (data->user_input[i] == '|')
		{
			if (status == WAITING_FOR_CHAR)
			{
				ft_printf("-minishell: syntax error near unexpected token `|'\n");
				return (258);
			}
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue ;
			else if (status == WAITING_FOR_PIPE)
			{
				code = create_command(data, start_i, i);
				if (code != 0)
				{
					if (code == -2)
						ft_printf("-minishell: syntax error near unexpected token `|'\n");
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
				continue ;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue ;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue ;
			else if (status == WAITING_FOR_PIPE)
				continue ;
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
				ft_printf("-minishell: syntax error near unexpected token `newline'\n");
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
}
