/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:44:14 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/16 15:34:57 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	t_data	*data;

	data = init_zero();
	while (1)
	{
		data->user_input = readline(PROMPT);
		if (data->user_input)
		{
			add_history(data->user_input);
			parse_input(data);
			builtins(data);
			free(data->user_input);
			data->user_input = NULL;
		}
	}
	return (0);
}
