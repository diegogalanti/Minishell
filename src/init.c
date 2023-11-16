/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:52:33 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/16 15:25:14 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    *init_zero()
{
	t_data	*data;

	data = 0;
	data = safe_malloc(data, sizeof(t_data));
	data->user_input = NULL;
	return data;
}