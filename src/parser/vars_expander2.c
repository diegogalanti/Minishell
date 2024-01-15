/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_expander2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:02:09 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 14:35:22 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_status	check_squote_ve(t_parse_status current, int *i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_DOLLAR)
		return (FOUND_SQOT_WFD);
	else if (current == FOUND_SQOT_WFD)
		return (WAITING_FOR_DOLLAR);
	else if (current == WAITING_FOR_SPACE)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (FOUND_SQOT_WFD);
	}
	else if (current == WFC)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (FOUND_SQOT_WFD);
	}
	else if (current == FOUND_DQOT)
		return (FOUND_DQOT_WFD);
	else if (current == FOUND_DQOT_WFS)
	{
		*i += expand_var(cmd, start_i, *i - 1, 1);
		return (FOUND_DQOT_WFD);
	}
	return (current);
}

t_parse_status	check_dquote_ve(t_parse_status current, int *i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_DOLLAR)
		return (FOUND_DQOT_WFD);
	else if (current == WAITING_FOR_SPACE)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (FOUND_DQOT_WFD);
	}
	else if (current == WFC)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (FOUND_DQOT_WFD);
	}
	else if (current == FOUND_DQOT_WFD)
		return (WAITING_FOR_DOLLAR);
	else if (current == FOUND_DQOT)
		return (WAITING_FOR_DOLLAR);
	else if (current == FOUND_DQOT_WFS)
	{
		*i += expand_var(cmd, start_i, *i - 1, 1);
		return (WAITING_FOR_DOLLAR);
	}
	return (current);
}

t_parse_status	check_question_ve(t_parse_status current, int *i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_SPACE)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (WAITING_FOR_DOLLAR);
	}
	else if (current == WFC)
	{
		*i += expand_exit_status(cmd, start_i, *i, 0);
		return (WAITING_FOR_DOLLAR);
	}
	else if (current == FOUND_DQOT)
	{
		*i += expand_exit_status(cmd, start_i, *i, 1);
		return (FOUND_DQOT_WFD);
	}
	else if (current == FOUND_DQOT_WFS)
	{
		*i += expand_var(cmd, start_i, *i - 1, 1);
		return (FOUND_DQOT_WFD);
	}
	return (current);
}

t_parse_status	check_space_ve(t_parse_status current, int *i,
	int start_i, t_command *cmd)
{
	if (current == WAITING_FOR_SPACE)
	{
		*i += expand_var(cmd, start_i, *i - 1, 0);
		return (WAITING_FOR_DOLLAR);
	}
	else if (current == WFC)
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
