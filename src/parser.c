/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2023/11/16 17:25:37 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_input(t_data *data)
{
	int	i;
	
	i = -1;
	while (data->user_input[++i])
	{
	}

	/* one block for each command */
	//first command
	t_command *command1 = safe_malloc(data, sizeof(t_command));
	command1->cmd = ECHO;
	command1->argv = safe_malloc(data, 8 * 3); /* 2 is the number of arguments for the command */
		command1->argv[0] = "echo";
		command1->argv[1] = "abc";
		command1->argv[2] = "$USER";
	command1->i_redirect = "/folde/input.txt"; /* set to 0 to use stdin */
	command1->o_redirect = "/folde/output.txt"; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	command1->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */

	//second command
	t_command *command2 = safe_malloc(data, sizeof(t_command));
	command2->cmd = CD;
	command2->argv = safe_malloc(data, 8 * 2); /* 2 is the number of arguments for the command */
		command2->argv[0] = "cd";
		command2->argv[1] = "/bin";
	command2->i_redirect = 0; /* set to 0 to use stdin */
	command2->o_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	command2->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */

	//for the first command
	data->commands = ft_lstnew(command1);

	//for every additional command
	ft_lstadd_back(&data->commands, ft_lstnew(command2));
}