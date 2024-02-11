/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:54:59 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/02/11 15:00:08 by digallar         ###   ########.fr       */
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

/* set_fd: assigns the right file descriptors for command
	according to redirections and pipes
	If it is the 1. command (i = 0) and there is no redirection
		(command->fd_in < 0) fd_in is set to stdin
	The same applies for the last command (i = nb_cmds - 1) and
		which is set to stdout if not an fd is set for redirection
	Else fd_in and fd_out are set to the right pipe end to write or
		read from it.
	Last, all other pipe ends are closed, so that the process
		is not waiting for them but exits when finished.
*/

int	set_fd(t_data *data, t_command *command, int i)
{
	if (i == 0 && command->fd_in < 0)
		command->fd_in = STDIN_FILENO;
	else if (command->fd_in < 0)
		command->fd_in = data->pipe[i - 1][0];
	if (i == data->nb_cmds - 1 && command->fd_out < 0)
		command->fd_out = STDOUT_FILENO;
	else if (command->fd_out < 0)
		command->fd_out = data->pipe[i][1];
	i = -1;
	while (++i < data->nb_cmds - 1)
	{
		if (data->pipe[i][0] != command->fd_in)
			close(data->pipe[i][0]);
		if (data->pipe[i][1] != command->fd_out)
			close(data->pipe[i][1]);
	}
	return (1);
}

void	child_process(t_data *data, t_command *command, int i)
{
	command->exit_status = 1;
	if ((command->fd_in == -1 && command->stdin)
		|| (command->fd_out == -1 && command->stdout))
	{
		command->fd_out = -1;
		exit_child(data, command, 1);
		return ;
	}
	set_fd(data, command, i);
	if (command->cmd == EXEC || command->cmd == NOT_FOUND)
		execute_command(data, command);
	else
	{
		command->exit_status = check_builtins(data, command, i);
	}
	exit_child (data, command, command->exit_status);
}

/* get_child_exit_status:
	stores the childs exit status in data->exit_status with which shell exits
	The child's exit status is only stored if child exited normally
		(checked with macro WIFEXITED)
	If it exited normally, the child's status is retrieved
		with the macro WEXITSTATUS*/

void	get_child_exit_status(t_data *data, int child_exit_status)
{
	if (WIFEXITED(child_exit_status))
	{
		data->exit_status = WEXITSTATUS(child_exit_status);
	}
}

int	pipe_commands(t_data *data)
{
	int		i;
	int		child_exit_status;
	t_list	*command;

	command = data->commands;
	if (!creat_pipe(data))
		return (printf("Minishell: Error: Could not create pipes\n"), 0);
	i = -1;
	while (++i < data->nb_cmds)
	{
		data->pid[i] = fork();
		if (data->pid[i] < 0)
			return (printf("Minishell: Error: Fork failed\n"), 0);
		if (data->pid[i] == 0)
			child_process(data, command->content, i);
		command = command->next;
	}
	close_all_fd(data);
	i = -1;
	while (++i < data->nb_cmds)
	{
		waitpid(data->pid[i], &child_exit_status, 0);
		get_child_exit_status(data, child_exit_status);
	}
	return (1);
}
