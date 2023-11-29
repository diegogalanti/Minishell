/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:54:59 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/28 17:53:33 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	creat_pipe(t_data *data)
{
	int	i;

	data->pipe = safe_malloc(data, (sizeof(int *) * (data->nb_cmds - 1)));
	data->pid = safe_malloc(data, (sizeof(int) * data->nb_cmds));
	if (!data->pipe || !data->pid)
		return (0);
	i = -1;
	while (++i < data->nb_cmds - 1)
	{
		data->pipe[i] = safe_malloc(data, (sizeof(int) * 2));
		if (!data->pipe[i])
			return (0);
	}
	i = -1;
	while (++i < data->nb_cmds - 1)
	{
		if (pipe(data->pipe[i]) < 0)
			return (0);
	}
	return (1);
}

int	set_fd(t_data *data, t_command *command, int i)
{
	if (i == 0)
		command->fd_in = STDIN_FILENO;
	else
		command->fd_in = data->pipe[i - 1][0];
	if (i == data->nb_cmds - 1)
		command->fd_out	= STDOUT_FILENO;
	else
		command->fd_out = data->pipe[i][1];
	i = -1;
	while (++i < data->nb_cmds - 1)
	{
		if (data->pipe[i][0] != command->fd_in)
		{
			if (close(data->pipe[i][0]) < 0)
				printf("Minishell: Error: close fd failed\n");
		}
		if (data->pipe[i][1] != command->fd_out)
		{
			if (close(data->pipe[i][1]) < 0)
				printf("Minishell: Error: close fd failed\n");
		}
	}
	return (1);
}

void	child_process(t_data *data, t_command *command, int i)
{
	set_fd(data, command, i);
	printf("fd_in: %i, fd_out: %i", command->fd_in, command->fd_out);
	redirect(data, command);
	if (command->cmd == EXEC)
		execute_command(data, command);
	else
	{
		redirect(data, command);
		check_builtins(data, command, i);
	}
	exit_child (1, data);
}

int	pipe_commands(t_data *data)
{
	int	i;
	t_list *command;

	command = data->commands;
	if (!creat_pipe(data))
		return (printf("Minishell: Error: Could not create pipes\n"), 0);
	i = -1;
	while (++i < data->nb_cmds)
	{
		if ((data->pid[i] = fork()) < 0)
			return (printf("Minishell: Error: Fork failed\n"), 0);
		if (data->pid[i] == 0)
			child_process(data, command->content, i);
		command = command->next;
	}
	close_all_fd(data);
	i = -1;
	while (++i < data->nb_cmds)
		waitpid(0, &data->exit_status, 0);
	return (1);
}
