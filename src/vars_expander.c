/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:02:09 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 08:49:47 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_dolar_ve(t_parse_status current, int *i,
	int *start_i, t_command *cmd)
{
	if (current == WAITING_FOR_DOLLAR)
	{
		*start_i = *i;
		return (WAITING_FOR_CHAR);
	}
	else if (current == WAITING_FOR_SPACE || current == WAITING_FOR_CHAR)
	{
		*i += expand_var(cmd, *start_i, *i - 1, 0);
		*start_i = *i;
		return (WAITING_FOR_CHAR);
	}
	else if (current == FOUND_DQOT_WFD)
	{
		*start_i = *i;
		return (FOUND_DQOT);
	}
	else if (current == FOUND_DQOT)
		*start_i = *i;
	else if (current == FOUND_DQOT_WFS)
	{
		*i += expand_var(cmd, *start_i, *i - 1, 1);
		*start_i = *i;
		return (FOUND_DQOT_WFD);
	}
	return (current);
}

static t_parse_status	check_special_ve(t_parse_status current, int *i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_SPACE)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (WAITING_FOR_DOLLAR);
	}
	else if (current == WAITING_FOR_CHAR)
		return (WAITING_FOR_DOLLAR);
	else if (current == FOUND_DQOT)
		return (FOUND_DQOT_WFD);
	else if (current == FOUND_DQOT_WFS)
	{
		*i += expand_var(cmd, start_i, *i - 1, 1);
		return (FOUND_DQOT_WFD);
	}
	return (current);
}

static void	check_final_ve(t_parse_status current, int i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_SPACE)
		expand_var(cmd, start_i, i - 1, 0);
}

static t_parse_status	check_other_ve(t_parse_status current)
{
	if (current == WAITING_FOR_CHAR)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_DQOT)
		return (FOUND_DQOT_WFS);
	return (current);
}

void	expand_vars(t_command *cmd)
{
	int				i;
	int				start_i;
	t_parse_status	status;

	i = -1;
	start_i = 0;
	status = WAITING_FOR_DOLLAR;
	while (cmd->cmd_input[++i])
	{
		if (cmd->cmd_input[i] == '\'')
			status = check_squote_ve(status, &i, start_i, cmd);
		else if (cmd->cmd_input[i] == '\"')
			status = check_dquote_ve(status, &i, start_i, cmd);
		else if (cmd->cmd_input[i] == '?')
			status = check_question_ve(status, &i, start_i, cmd);
		else if (ft_isspace(cmd->cmd_input[i]))
			status = check_space_ve(status, &i, start_i, cmd);
		else if (cmd->cmd_input[i] == '$')
			status = check_dolar_ve(status, &i, &start_i, cmd);
		else if (!ft_isalnum(cmd->cmd_input[i]) && cmd->cmd_input[i] != '_')
			status = check_special_ve(status, &i, start_i, cmd);
		else
			status = check_other_ve(status);
	}
	check_final_ve(status, i, start_i, cmd);
}
