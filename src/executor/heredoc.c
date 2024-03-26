/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:47:46 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 16:29:59 by tstahlhu         ###   ########.fr       */
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
		return (ft_putstr_fd("minishell: .here_doc \n", STDERR_FILENO), 0);
	while (1)
	{
		buf = readline("< ");
		if (strncmp(buf, cmd->limiter, ft_strlen(buf)) == 0)
			break ;
		write(heredoc, buf, ft_strlen(buf));
		write(heredoc, "\n", 1);
		free(buf);
	}
	free(buf);
	close(heredoc);
	cmd->fd_in = open(".here_doc", O_RDONLY);
	return (1);
}
