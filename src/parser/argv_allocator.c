/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_allocator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:53:29 by digallar          #+#    #+#             */
/*   Updated: 2024/01/23 10:18:19 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_dquote_aa(t_parse_status current)
{
	if (current == WFC)
		return (FOUND_DQOT_WFC);
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

static t_parse_status	check_squote_aa(t_parse_status current)
{
	if (current == WFC)
		return (FOUND_SQOT_WFC);
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

static t_parse_status	check_space_aa(t_parse_status current, int *size)
{
	if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	else if (current == WAITING_FOR_SPACE)
	{
		(*size)++;
		return (WFC);
	}
	return (current);
}

static t_parse_status	check_other_aa(t_parse_status current)
{
	if (current == WFC)
		return (WAITING_FOR_SPACE);
	else if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	return (current);
}

void	alloc_argv(t_command *command)
{
	int				i;
	int				size;
	t_parse_status	status;

	i = -1;
	size = 0;
	status = WFC;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\"')
			status = check_dquote_aa(status);
		else if (command->cmd_input[i] == '\'')
			status = check_squote_aa(status);
		else if (ft_isspace(command->cmd_input[i]))
			status = check_space_aa(status, &size);
		else
			status = check_other_aa(status);
	}
	if (status == WAITING_FOR_SPACE)
		size++;
	//ft_printf("Command has %i arguments.\n", size);
	command->argv = command_safe_malloc(command,
			sizeof(command->argv) * (size + 1));
	command->argv[size] = 0;
}
