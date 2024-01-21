/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:53:05 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/21 17:48:57 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* set_redirections: checks for redirections and opens files if necessary
	if an input file is found (stdin), 
		the file is opened and the fd stored in cmd->fd_in
	if a limiter is found, a heredoc is created and the fd stored in cmd->fd_in
	if an output file is found (stdout) it is either opened in append or 
		truncate mode (depending on append_mode set in parser), 
		the fd is stored in cmd->fd_out
	if no file is given, the fd_in and/ or fd_out is set to -1 
		(this value is also used for closed files)*/

int	set_redirections(t_command *c)
{
	if (c->stdin)
	{
		c->fd_in = open(c->stdin, O_RDONLY);
		if (c->fd_in == -1)
			return (printf("minishell: %s: %s\n",
					c->stdin, strerror(errno)), 0);
	}
	else if (c->limiter)
		here_doc(c);
	else
		c->fd_in = -1;
	if (c->stdout)
	{
		if (c->append_mode == 0)
			c->fd_out = open(c->stdout, O_CREAT | O_WRONLY | O_TRUNC, 00664);
		else if (c->append_mode == 1)
			c->fd_out = open(c->stdout, O_CREAT | O_WRONLY | O_APPEND, 00664);
		if (c->fd_out == -1)
			return (printf("minishell: %s: %s\n", c->stdout, strerror(errno)),
				0);
	}
	else
		c->fd_out = -1;
	return (1);
}

int	close_redirections(t_command *cmd)
{
	if (cmd->stdin)
		close_fd(&cmd->fd_in);
	if (cmd->stdout)
		close_fd(&cmd->fd_out);
	if (cmd->limiter)
	{
		unlink(".here_doc");
		close_fd(&cmd->fd_in);
	}
	return (1);
}

int	check_redirections(t_data *data, int (*f)(t_command *))
{
	int		i;
	t_list	*head;

	head = data->commands;
	i = -1;
	while (++i < data->nb_cmds)
	{
		if (!(*f)(data->commands->content))
		{
			data->exit_status = errno;
			data->commands = head;
			return (0);
		}
		data->commands = data->commands->next;
	}
	data->commands = head;
	return (1);
}

/*redirect: redirects the input and / or output if necessary
	1. stores stdin and stdout as copies in data
	2. if fd_in is set stdinput is redirected to it,
		fd_in is closed (because dup2 copied it)
	3- if fd_out is set, stdoutput is redirected to it,
		fd_out is closed (because dup2 copied it)
	TO DO: close or close_fd? close_fd sets fd to -1 -> problematic
		close could result in a close failure later
	CHANGED: stdin and stdout cpy were standing alone at the beginning,
	i.e. were always copied. now they are only copied if there is a redirection	
		*/
int	redirect(t_command *command, t_data *data)
{
	if (data->nb_cmds > 1 && data->stdout_cpy == -1)
		data->stdout_cpy = dup(STDOUT_FILENO);
	if (command->fd_in != STDIN_FILENO && command->fd_in >= 0)
	{
		if (data->stdin_cpy == -1)
			data->stdin_cpy = dup(STDIN_FILENO);
		dup2(command->fd_in, STDIN_FILENO);
		close_fd(&command->fd_in);
	}
	if (command->fd_out != STDOUT_FILENO && command->fd_out >= 0)
	{
		if (data->stdout_cpy == -1)
			data->stdout_cpy = dup(STDOUT_FILENO);
		dup2(command->fd_out, STDOUT_FILENO);
		close_fd(&command->fd_out);
	}
	return (1);
}

void	undirect(t_command *command, t_data *data)
{
	if (command->fd_in > -1 && data->stdin_cpy > -1)
	{
		dup2(data->stdin_cpy, STDIN_FILENO);
		close_fd(&(data->stdin_cpy));
	}
	if (command->fd_out > -1 && data->stdout_cpy > -1)
	{
		dup2(data->stdout_cpy, STDOUT_FILENO);
		close_fd(&(data->stdout_cpy));
	}
}
