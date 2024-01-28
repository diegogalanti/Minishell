/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_builder2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:44:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/28 16:21:23 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_squote_rb(t_parse_status current)
{
	if (current == WAITING_FOR_RED)
		return (FOUND_SQOT);
	else if (current == FOUND_SQOT)
		return (WAITING_FOR_RED);
	else if (current == FOUND_IRED)
		return (FOUND_SQOT_WFS);
	else if (current == FOUND_ORED)
		return (FOUND_SQOT_WFS);
	else if (current == FOUND_SQOT_WFS)
		return (WAITING_FOR_SPACE);
	else if (current == WFC)
		return (FOUND_SQOT_WFS);
	else if (current == WAITING_FOR_SPACE)
		return (FOUND_SQOT_WFS);
	return (current);
}

static t_parse_status	check_dquote_rb(t_parse_status current)
{
	if (current == WAITING_FOR_RED)
		return (FOUND_DQOT);
	else if (current == FOUND_DQOT)
		return (WAITING_FOR_RED);
	else if (current == FOUND_IRED)
		return (FOUND_DQOT_WFS);
	else if (current == FOUND_ORED)
		return (FOUND_DQOT_WFS);
	else if (current == FOUND_DQOT_WFS)
		return (WAITING_FOR_SPACE);
	else if (current == WFC)
		return (FOUND_DQOT_WFS);
	else if (current == WAITING_FOR_SPACE)
		return (FOUND_DQOT_WFS);
	return (current);
}

t_parse_status	check_space_rb(t_parse_status current, int start_i,
	int *i, t_command *command)
{
	if (current == FOUND_IRED)
		return (WFC);
	else if (current == FOUND_ORED)
		return (WFC);
	else if (current == WAITING_FOR_SPACE)
	{
		*i += create_redirection(command, start_i, *i);
		return (WAITING_FOR_RED);
	}
	return (current);
}

static t_parse_status	check_other_rb(t_parse_status current)
{
	if (current == FOUND_ORED)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_IRED)
		return (WAITING_FOR_SPACE);
	else if (current == WFC)
		return (WAITING_FOR_SPACE);
	return (current);
}

t_parse_status	check_quotesother_rb(char c, t_parse_status status)
{
	if (c == '\'')
		return (check_squote_rb(status));
	else if (c == '\"')
		return (check_dquote_rb(status));
	else
		return (check_other_rb(status));
}
