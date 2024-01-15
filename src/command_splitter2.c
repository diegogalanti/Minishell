/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_splitter2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 10:19:57 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 08:55:32 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_status	check_dquote_cs(t_parse_status current)
{
	if (current == WAITING_FOR_CHAR)
		return (FOUND_DQOT_WFC);
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_SQOT)
		return (current);
	else if (current == FOUND_DQOT_WFC)
		return (WAITING_FOR_PIPE);
	else if (current == FOUND_DQOT)
		return (WAITING_FOR_PIPE);
	else if (current == WAITING_FOR_PIPE)
		return (FOUND_DQOT);
	return (current);
}

t_parse_status	check_squote_cs(t_parse_status current)
{
	if (current == WAITING_FOR_CHAR)
		return (FOUND_SQOT_WFC);
	else if (current == FOUND_SQOT_WFC)
		return (WAITING_FOR_PIPE);
	else if (current == FOUND_SQOT)
		return (WAITING_FOR_PIPE);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	else if (current == FOUND_DQOT)
		return (current);
	else if (current == WAITING_FOR_PIPE)
		return (FOUND_SQOT);
	return (current);
}
