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

/*int	here_doc(t_data *data)
{
	char	*buf;
	int		heredoc;

	heredoc = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 00664);
	if (heredoc < 0)
		return (printf("minishell: heredoc: open failed\n"), 0);
	while (1)
	{
		ft_printf("> ");
		buf = get_next_line(0);
		if (strncmp(buf, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(heredoc, buf, ft_strlen(buf));
		write(heredoc, "/n", 1);
		free(buf);
	}
	free(buf);
	close(heredoc);
	error_check(data->f1 = open(".here_doc", O_RDONLY), ".here_doc", data);
}*/

/*int    redirect(t_data *data)
{
	if (data->i_redirect)
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
	}
}*/