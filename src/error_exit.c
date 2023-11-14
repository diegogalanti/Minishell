/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:44:27 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/14 16:02:23 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_exit(t_data **data)
{
	if ((*data)->user_input)
	{
		free((*data)->user_input);
		(*data)->user_input = NULL;
	}
	clear_history();
	exit (0);
}

