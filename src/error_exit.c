/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:44:27 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 16:52:44 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_list(void *list)
{
	free(list);
}

void	free_data(t_data *data)
{
	if (data && data->free_list)
	{
		ft_lstclear(&(data->free_list), clear_list);
	}
	if (data->user_input)
	{
		free(data->user_input);
		data->user_input = NULL;
	}
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

void	free_command(t_command *command)
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
	int	exit_status;

	exit_status = data->exit_status;
	close_all_fd(data);
	free_data(data);
	clear_history();
	exit (exit_status);
}

void	exit_child(t_data *data, t_command *command, int exit_status)
{
	undirect(command, data);
	close_all_fd(data);
	clear_history();
	exit (exit_status);
}

void	print_error(char *where, char *what, char *why)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (*where != '\0')
	{
		ft_putstr_fd(where, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (*what != '\0')
	{
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(what, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
	ft_putstr_fd(why, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
