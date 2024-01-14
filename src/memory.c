/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:51:58 by digallar          #+#    #+#             */
/*   Updated: 2024/01/14 10:01:05 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(t_data *data, size_t size)
{
	void	*ptr;
	t_list	*new;

	ptr = malloc(size);
	if (!ptr)
		return (ft_printf("Error: Could not malloc.\n"),
			free_exit(data),
			(void *) 0);
	if (!data)
		return (ptr);
	if (!data->free_list)
	{
		data->free_list = ft_lstnew(ptr);
		if (!data->free_list)
			return (free_exit(data), (void *) 0);
	}
	else
	{
		new = ft_lstnew(ptr);
		if (!new)
			return (free_exit(data), (void *) 0);
		ft_lstadd_front(&(data->free_list), new);
	}
	return (ptr);
}

/* frees an array of strings
	To Do: list free has to be added so that there is no double free when calling free_data*/

/*void	*free_arr_str(char **str)
{
	int	i;

	i = -1;
	if (str == NULL)
		return (NULL);
	while (str[++i])
	{
		free(str[i]);
		str[i] = NULL;
	}
	free(str);
	return (NULL);
}*/

void	*command_safe_malloc(t_command *command, size_t size)
{
	void	*ptr;
	t_list	*new;

	ptr = malloc(size);
	if (!ptr)
		return (ft_printf("Error\nCould not malloc.\n"),
			free_command(command),
			(void *) 0);
	if (!command)
		return (ptr);
	if (!command->free_list)
	{
		command->free_list = ft_lstnew(ptr);
		if (!command->free_list)
			return (free_command(command), (void *) 0);
	}
	else
	{
		new = ft_lstnew(ptr);
		if (!new)
			return (free_command(command), (void *) 0);
		ft_lstadd_front(&(command->free_list), new);
	}
	return (ptr);
}

void	close_fd(int *fd)
{
	if (*fd != -1)
	{
		if (close(*fd) < 0)
			printf("Minishell: Error: closing fd %i failed\n", *fd);
		*fd = -1;
	}
}

/* closes all file descriptors
	so far, only the ones for the pipes
	To Do: close heredoc & files from redirection*/
int	close_all_fd(t_data *data)
{
	int	i;

	i = -1;
	if (data->pipe)
	{
		while (++i < data->nb_cmds - 1)
		{
			if (data->pipe[i][0])
				close_fd(&data->pipe[i][0]);
			if (data->pipe[i][1])
				close_fd(&data->pipe[i][1]);
		}
	}
	check_redirections(data, close_redirections);
	close_fd(&data->stdout_cpy);
	return (1);
}
