/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_multiple.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:04:29 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/21 17:53:06 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* find_mult_redir (find multiple redirections)
	minishell can (as bash) only handle one input and one output redirection
	if the user inputs multiple redirections, e.g. ls > file1 > file2
	the output is redirected to the last file (e.g. file2)
	file1 is, however, created
	find_mult_redir scans for these addtional (output) redirections */

int	find_mult_redir(char **argv)
{
	int	i;
	int	fd;
	int	r;

	r = 0;
	i = 1;
	while (argv[i] && argv[i] != NULL)
	{
		if (!ft_strncmp(argv[i], ">", 1))
		{
			fd = open(argv[++i], O_CREAT | O_WRONLY | O_TRUNC, 00664);
			close(fd);
			r++;
		}
		i++;
	}
	return (r);
}

/* trunc_mult_redir (truncate multiple redirections)
	if there are multiple (output) redirections, 
	this functions truncates them so that execve functions properly.
	The parser stores additional redirections in command->argv[],
	these additional strings are deleted. */

void	trunc_mult_redir(char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && argv[i] != NULL)
	{
		if (!ft_strncmp(argv[i], ">", 1))
		{
			while (argv[i] && argv[i] != NULL)
			{
				free(argv[i]);
				argv[i] = NULL;
				i++;
			}
		}
		else
			i++;
	}
}
