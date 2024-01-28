/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2024/01/28 15:44:58 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_commented(t_data *data)
{
	int	i;

	i = 0;
	while (ft_isspace(data->user_input[i]))
	{
		i++;
	}
	if (data->user_input[i] == '#')
		return (1);
	return (0);
}

static void	init_commands(t_data *data)
{
	data->nb_cmds = 0;
	data->commands = 0;
}

void	parse_input(t_data *data)
{
	init_commands(data);
	split_commands(data);
}
