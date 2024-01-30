/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:44:14 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/29 18:30:04 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_data	*data;

	if (argc > 1)
		printf("implement non-interactive mode for %s\n", *argv);
	data = init_data(env);
	while (1)
	{
		data->user_input = readline(PROMPT);
		if (data->user_input)
		{
			add_history(data->user_input);
			if (!check_commented(data))
			{
				parse_input(data);
				execute(data);
                  //  reset_data(data);
			}
			free(data->user_input);
			data->user_input = NULL;
			close_all_fd(data);
		}
        else
            sleep (3);
	}
	return (0);
}
