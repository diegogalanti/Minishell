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
	data->stdout_cpy = dup(STDOUT_FILENO);
	return (1);
}

int	set_fd(t_data *data, t_command *command, int i)
{
	if (i == 0 && command->fd_in < 0)
		command->fd_in = STDIN_FILENO;
	else if (command->fd_in < 0)
		command->fd_in = data->pipe[i - 1][0];
	if (i == data->nb_cmds - 1 && command->fd_out < 0)
		command->fd_out	= STDOUT_FILENO;
	else if (command->fd_out < 0)
		command->fd_out = data->pipe[i][1];
	i = -1;
	while (++i < data->nb_cmds - 1)
	{
		if (data->pipe[i][0] != command->fd_in)
			close_fd(&data->pipe[i][0]);
		if (data->pipe[i][1] != command->fd_out)
			close_fd(&data->pipe[i][1]);
	}
	return (1);
}

void	child_process(t_data *data, t_command *command, int i)
{
	set_fd(data, command, i);
	//printf("fd_in: %i, fd_out: %i\n", command->fd_in, command->fd_out);
	if (command->cmd == EXEC)
		execute_command(data, command);
	else
	{
		check_builtins(data, command, i);
		undirect(command, data);
	}
	exit_child (data, 1);
}

/* get_child_exit_status: stores the childs exit status in data->exit_status with which shell exits
	The child's exit status is only stored if child exited normally (checked with macro WIFEXITED)
	If it exited normally, the child's status is retrieved with the macro WEXITSTATUS*/

void	get_child_exit_status(t_data *data, int child_exit_status)
{
	if (WIFEXITED(child_exit_status))
	{
		data->exit_status = WEXITSTATUS(child_exit_status);
		//printf("Exit status of child was: %i, %i\n", child_exit_status, data->exit_status);
	}
}

int	pipe_commands(t_data *data)
{
	int	i;
	int	child_exit_status;
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
	{
		waitpid(0, &child_exit_status, 0);
		get_child_exit_status(data, child_exit_status);
	}
	return (1);
}
