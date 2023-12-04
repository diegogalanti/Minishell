/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:53:05 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/21 14:53:07 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* here_doc: This function mimics the behaviour of "<< LIMITER" in a shell.
	A temporary file called "here_doc" is opened. With the help of the function
	get_next_line the user input is read line by line into a buffer and written
	into the here_doc file until a LIMITER (set by the user) is hit. After 
	its creation the here_doc file is treated the same as the infile (f1).*/

int	here_doc(t_command *cmd)
{
	char	*buf;
	int		heredoc;

	heredoc = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 00664);
	if (heredoc < 0)
		return (printf("minishell: .here_doc \n"), 0);
	while (1)
	{
		//ft_printf("> ");
		//buf = get_next_line(0);
		buf = readline("< ");
		if (strncmp(buf, cmd->limiter, ft_strlen(cmd->limiter)) == 0)
			break ;
		write(heredoc, buf, ft_strlen(buf));
		write(heredoc, "/n", 1);
		free(buf);
	}
	free(buf);
	close(heredoc);
	cmd->fd_in = open(".here_doc", O_RDONLY);
	return (1);
}

/*int	here_doc(t_command *cmd)
{
	char	*buf;

	cmd->fd_in = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 00664);
	if (cmd->fd_in < 0)
		return (printf("minishell: .here_doc: %s\n", strerror(errno)), 0);
	while (1)
	{
		buf = readline("< ");
		if (ft_strncmp(buf, cmd->limiter, ft_strlen(cmd->limiter)) == 0)
			break ;
		write(cmd->fd_in, buf, ft_strlen(buf));
		write(cmd->fd_in, "/n", 1);
		free(buf);
	}
	free(buf);
	return (1);
}*/

int	set_redirections(t_command *cmd)
{
	if (cmd->stdin)
	{
		cmd->fd_in = open(cmd->stdin, O_RDONLY);
		if (cmd->fd_in == -1)
			return (printf("minishell: %s: %s\n", cmd->stdin, strerror(errno)), 0);
	}
	else if (cmd->limiter)
		here_doc(cmd);
	else
		cmd->fd_in = 0;
	if (cmd->stdout)
	{
		if (cmd->append_mode == 0)
			cmd->fd_out = open(cmd->stdout, O_CREAT | O_WRONLY | O_TRUNC, 00664);
		else if (cmd->append_mode == 1)
			cmd->fd_out = open(cmd->stdout, O_CREAT | O_WRONLY | O_APPEND, 00664);
		if (cmd->fd_out == -1)
			return (printf("minishell: %s: %s\n", cmd->stdout, strerror(errno)), 0);
	}
	else
		cmd->fd_out = 0;
	return (1);
}

int    check_redirections(t_data *data)
{
	int	i;
	t_list	*head;

	head = data->commands;
	i = -1;
	while (++i < data->nb_cmds)
	{
		if (!set_redirections(data->commands->content))
		 {
			data->exit_status = errno;
			return (0);
		 }
		data->commands = data->commands->next;		
	}
	data->commands = head;
	return (1);
	/*if (data->i_redirect)
	{
		if (heredoc == 1)
			if (heredoc(data) < 0)
				return (-1);
		else
			open file. store fd in first command fd_in
	}
	if (data->o_redirect)
	{
		open file. store fd in last command fd_out
		O_TRUNC or O_APPEND (just use variable from data)
	}*/
}

int	redirect(t_command *command)
{
	//if (!command->stdin && !command->stdout)
	//	return (0);
	if (command->fd_in != STDIN_FILENO && command->fd_in >= 0)
	{
		dup2(command->fd_in, STDIN_FILENO);
		close_fd(&(command)->fd_in);
	}
	if (command->fd_out != STDOUT_FILENO && command->fd_out >= 0)
	{
		dup2(command->fd_out, STDOUT_FILENO);
		close_fd(&(command)->fd_out);
	}
	return (1);
}