/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:53:29 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 09:54:13 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_dquote_ab(t_parse_status current,
	int *start_i, int i)
{
	if (current == WAITING_FOR_CHAR)
	{
		*start_i = i;
		return (FOUND_DQOT_WFC);
	}
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_DQOT)
		return (WAITING_FOR_SPACE);
	else if (current == WAITING_FOR_SPACE)
		return (FOUND_DQOT);
	return (current);
}

static t_parse_status	check_squote_ab(t_parse_status current,
	int *start_i, int i)
{
	if (current == WAITING_FOR_CHAR)
	{
		*start_i = i;
		return (FOUND_SQOT_WFC);
	}
	else if (current == FOUND_SQOT_WFC)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_SQOT)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	else if (current == WAITING_FOR_SPACE)
		return (FOUND_SQOT);
	return (current);
}

static t_parse_status	check_other_ab(t_parse_status current,
	int *start_i, int i)
{
	if (current == WAITING_FOR_CHAR)
	{
		*start_i = i;
		return (WAITING_FOR_SPACE);
	}
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	return (current);
}

t_parse_status	check_nonspace_ab(t_parse_status current, int *start_i,
	int i, char c)
{
	if (c == '\"')
		return (check_dquote_ab(current, start_i, i));
	else if (c == '\'')
		return (check_squote_ab(current, start_i, i));
	else
		return (check_other_ab(current, start_i, i));
}
