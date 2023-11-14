/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:44:14 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/14 16:32:24 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	t_data	data;

	init_zero(&data);
	while (1)
	{
		if (data.user_input)
		{
			free(data.user_input);
			data.user_input = NULL;
		}
		data.user_input = readline(PROMPT);
		if (data.user_input && *data.user_input)
		{
			add_history(data.user_input);
			builtins(&data);
		}
	}
	free(data.user_input);
	return (0);
}
