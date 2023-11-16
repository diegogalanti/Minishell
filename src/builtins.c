/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:46:01 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/16 15:17:17 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    builtins(t_data *data)
{
	if (!ft_strncmp(data->user_input, "exit", 4))
		free_exit(data);
}
