/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:52:33 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/23 09:33:35 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* init_env: initializes the environmen variable by copying current env passed
			to main (this seems to do bash, too)*/

int	init_env(t_data *data, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
		i++;
	if (!(data->env = safe_malloc(data, (i + 1) * sizeof(char *))))
		return (0);
	i = -1;
	while (env[++i] != NULL)
		data->env[i] = fs_strdup(data, env[i]);
	data->env[i] = NULL;
	return (1);
}

/* data struct is initialized
	stdout_cpy is set to -1 so that it will not attempt to be close if unititialized*/

void   *init_data(char **env)
{
	t_data	*data;

	data = 0;
	data = safe_malloc(data, sizeof(t_data));
	data->user_input = NULL;
	data->shvar = NULL;
	data->free_list = NULL;
	data->commands = NULL;
	data->nb_cmds = 0;
	data->pid = NULL;
	data->pipe = NULL;
	data->exit_status = 0;
	data->stdout_cpy = -1;
	data->stdin_cpy = -1;
	if (!init_env(data, env))
		return (printf("Error: Could not initialize environment variable\n"),
				free_exit(data), (void *) 0);
	return (data);
}