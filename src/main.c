/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:44:14 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 14:53:55 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal[2] = {-1, -1};

/* Maybe implement (This is not required by the subject)
	environment variables: if "some_var=some_value" add it to data->shvar
	this step should come after builtins and commands were checked:
	while (command->argv[++i] != NULL)
		{
			if (check_export_var(command->argv[i]))
				data->shvar = add_var(data, data->shvar, command->argv[i]);
		}*/

int	main(int argc, char **argv, char **env)
{
	t_data				*data;

	set_signal();
	if (argc > 1)
		return (printf("implement non-interactive mode for %s\n", *argv), 0);
	data = init_data(env);
	while (1)
	{
		reset_g_signal();
		data->user_input = readline(PROMPT);
		if (g_signal[1] == 1)
			data->exit_status = 1;
		if (data->user_input)
		{
			add_history(data->user_input);
			if (!check_commented(data))
			{
				parse_input(data);
				g_signal[0] = 1;
				execute(data);
				if (g_signal[1] != -1 && g_signal[1] != 1)
					data->exit_status = g_signal[1];
			}
			free(data->user_input);
			data->user_input = NULL;
			close_all_fd(data);
		}
		else
		{
			printf("exit\n");
			free_exit(data);
		}
	}
	return (0);
}
