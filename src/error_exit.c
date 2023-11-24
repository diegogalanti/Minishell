/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:44:27 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/21 11:10:33 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_list(void *list)
{
	free(list);
}

void    free_data(t_data *data)
{
//	int	i;

	if (data && data->free_list)
	{
		ft_lstclear(&(data->free_list), clear_list);
	}
	if (data->user_input)
	{
		free(data->user_input);
		data->user_input = NULL;
	}
	/* tat added data->env because it is allocated with ft_split for test purposes,
	can be deleted as soon as diego has the parser finished */
	/*i = -1;
	if (data && data->env && data->env[0])
	{
		while (data->env[++i])
		{
			free(data->env[i]);
			data->env[i] = NULL;
		}
		free(data->env);
		data->env = NULL;
	}*/
	clear_history();
	exit (0);
}

void    free_command(t_command *command)
{
	if (command && command->free_list)
	{
		ft_lstclear(&(command->free_list), clear_list);
	}
	if (command->cmd_input)
	{
		free(command->cmd_input);
		command->cmd_input = NULL;
	}
	exit (0);
}

void	free_exit(t_data *data)
{
	free_data(data);
	clear_history();
	exit (0);
}

void	exit_child(int exit_status, t_data *data)
{
	close_all_fd(data);
	clear_history();
	exit (exit_status);
}
