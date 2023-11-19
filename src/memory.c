/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:51:58 by digallar          #+#    #+#             */
/*   Updated: 2023/11/16 15:26:52 by digallar         ###   ########.fr       */
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

void	*free_arr_str(char **str)
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
}