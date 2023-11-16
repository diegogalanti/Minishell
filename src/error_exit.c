/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:44:27 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/16 15:22:42 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_list(void *list)
{
	free(list);
}

void    free_exit(t_data *data)
{
	if (data && data->free_list)
	{
		ft_lstclear(&(data->free_list), clear_list);
	}
	if (data->user_input)
	{
		free(data->user_input);
		data->user_input = NULL;
	}
	clear_history();
	exit (0);
}

