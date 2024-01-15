/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:44:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 08:45:45 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_limiter(t_command *cmd, int start, int end)
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

static int	get_redirection_type(t_command *command, int start, int *i)
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

static int	create_redirection(t_command *cmd, int start, int end)
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

int	build_redirections(t_command *command, int next_c)
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
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), 258);
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
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), 258);
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
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), 258);
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
				return (ft_printf("-minishell: syntax error near unexpected token `%c'\n", command->cmd_input[i]), 258);
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
	if ((status == FOUND_ORED || status == FOUND_IRED || status == WAITING_FOR_CHAR) && next_c == 0)
		return (ft_printf("-minishell: syntax error near unexpected token `|'\n"), 258);
	else if ((status == FOUND_ORED || status == FOUND_IRED || status == WAITING_FOR_CHAR) && next_c == 1)
		return (ft_printf("-minishell: syntax error near unexpected token `newline'\n"), 258);
	else if (status == WAITING_FOR_SPACE)
		create_redirection(command, start_i, i);
	return (0);
}
