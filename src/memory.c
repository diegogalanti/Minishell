/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:51:58 by digallar          #+#    #+#             */
/*   Updated: 2023/11/14 18:00:41 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(t_data *data, size_t size)
{
	void	*ptr;
	t_list	*new;

	ptr = malloc(size);
	if (!ptr)
		return (ft_printf("Error\nCould not malloc.\n"),
			free_exit(&data),
			(void *) 0);
	if (!data)
		return (ptr);
	if (!data->free_list)
	{
		data->free_list = ft_lstnew(ptr);
		if (!data->free_list)
			return (free_exit(&data), (void *) 0);
	}
	else
	{
		new = ft_lstnew(ptr);
		if (!new)
			return (free_exit(&data), (void *) 0);
		ft_lstadd_front(&(data->free_list), new);
	}
	return (ptr);
}