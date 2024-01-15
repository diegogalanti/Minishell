/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_allocator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:53:29 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 09:01:21 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
