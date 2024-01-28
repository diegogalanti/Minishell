/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:52:33 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/28 16:53:33 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* init_env: initializes the environmen variable by copying current env passed
			to main (this seems to do bash, too)*/

int	init_env(t_data *data, char **env)
{
	int     shlvl;
	char    *c;

	data->env = env;
	if (!ft_strvcmp(find_var(data->env, "SHELL="), "minishell"))
	{
		shlvl = ft_atoi(find_var(data->env, "SHLVL="));
		if (shlvl >= 5000000)
		{
			printf("minishell: warning: shell level (%i) too high, resetting to 1\n", shlvl);
			shlvl = 0;
		}
		c = fs_itoa((shlvl + 1), data);
		add_mod_var(data, ff_strjoin("SHLVL=", c, data));
	}
	else
	{
		add_mod_var(data, ff_strjoin("SHELL=", "minishell", data));
	}
	return (1);
}

/*int	init_env(t_data *data, char **env)
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
}*/


/* data struct is initialized
	stdout_cpy is set to -1 
	so that it will not attempt to be close if unititialized*/

void	*init_data(char **env)
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
