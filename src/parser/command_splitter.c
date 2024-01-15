/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_splitter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 10:19:57 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 14:34:46 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_space_cs(t_parse_status current)
{
	if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	return (current);
}

static t_parse_status	check_other_cs(t_parse_status current)
{
	if (current == WFC)
		return (WAITING_FOR_PIPE);
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	return (current);
}

static t_parse_status	check_pipe_cs(t_data *data, int *start_i, int i,
	t_parse_status current)
{
	if (current == WFC)
	{
		ft_printf("-minishell: syntax error near unexpected token `|'\n");
		data->exit_status = 258;
		return (ABORT);
	}
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	else if (current == WAITING_FOR_PIPE)
	{
		create_command(data, *start_i, i, 0);
		if (data->exit_status != 0)
			return (ABORT);
		*start_i = i + 1;
		return (WFC);
	}
	return (current);
}

static void	check_final_cs(t_data *data, int start_i, int i,
	t_parse_status status)
{
	if (status == WAITING_FOR_PIPE)
	{
		create_command(data, start_i, i, 1);
		if (data->exit_status != 0)
			return ;
		data->nb_cmds++;
	}
}

void	split_commands(t_data *data)
{
	int				i;
	t_parse_status	status;
	int				start_i;

	i = -1;
	start_i = 0;
	status = WFC;
	while (data->user_input[++i])
	{
		if (data->user_input[i] == '\"')
			status = check_dquote_cs(status);
		else if (data->user_input[i] == '\'')
			status = check_squote_cs(status);
		else if (data->user_input[i] == '|')
		{
			if (check_pipe_cs(data, &start_i, i, status) == ABORT)
				return ;
		}
		else if (ft_isspace(data->user_input[i]))
			status = check_space_cs(status);
		else
			status = check_other_cs(status);
	}
	check_final_cs(data, start_i, i, status);
}
