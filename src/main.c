/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:44:14 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/04/08 11:09:46 by tstahlhu         ###   ########.fr       */
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
	data = init_data(env);
	if (argc > 1)
		non_interactive(argc, argv, data);
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

void	non_interactive(int argc, char **argv, t_data *data)
{
	int		i;
	char	*temp;

	data->user_input = ft_strdup(argv[1]);
	i = 1;
	while (++i < argc)
	{
		temp = ft_strjoin(data->user_input, " ");
		if (data->user_input)
			free(data->user_input);
		data->user_input = ft_strjoin(temp, argv[i]);
		if (temp)
			free(temp);
	}
	parse_input(data);
	execute(data);
	free_exit(data);
}
