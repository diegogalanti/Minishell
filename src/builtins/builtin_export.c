/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:38:52 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/30 12:46:03 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* env_var_name_check: checks if environment variable name is ok
	it seems only allowed characters (defined by POSIX) are:
	[a-zA-Z_][a-zA-Z0-9_]
	which means lowercase and uppercase letters and "_" and
	digits (but not in 1. place
	if it is ok, function returns 1
	if it is not ok, function returns 0 */

int	check_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '=' && str[i] != '+')
	{
		if (str[i] == '_')
			i++;
		else if ('A' <= str[i] && str[i] <= 'Z')
			i++;
		else if ('a' <= str[i] && str[i] <= 'z')
			i++;
		else if (('0' <= str[i] && str[i] <= '9') && i > 0)
			i++;
		else
			return (0);
	}
	while (str[i] != '\0')
	{
		if (32 <= str[i] && str[i] < 127)
			i++;
		else
			return (0);
	}
	return (1);
}

void	add_mod_var(t_data *data, char *var)
{
	int	i;

	i = 0;
	while (var[i] != '=' && var[i] != '+' && var[i] != '\0')
		i++;
	if (var[i] == '\0')
	{
		if (!is_var(data->shvar, var) && !is_var(data->env, var))
			data->shvar = add_var(data, data->shvar, var);
		return ;
	}
	else if (!is_var(data->env, var))
	{
		data->env = add_var(data, data->env, var);
		if (is_var(data->shvar, var))
			data->shvar = del_var(data, data->shvar, var);
	}
	else
		data->env = mod_var(data, data->env, var, i);
	return ;
}

void	print_export(char **env)
{
	int	i;
	int	j;

	i = -1;
	while (env && env[++i] != NULL)
	{
		printf("declare -x ");
		j = -1;
		while (env[i][++j] != '=' && env[i][j] != '\0')
			printf("%c", env[i][j]);
		if (env[i][j])
		{
			printf("%c%c", env[i][j++], 34);
			while (env[i][j] != '\0')
				printf("%c", env[i][j++]);
			printf("%c", 34);
		}
		printf("\n");
	}
}

void	builtin_export(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	if (command->argv[1] == NULL && data->env)
	{
		print_export(data->env);
		print_export(data->shvar);
		return ;
	}
	while (command->argv[++i] != NULL)
	{
		if (check_var_name(command->argv[i]))
			add_mod_var(data, command->argv[i]);
		else
		{
			printf("minishell: export: '%s': not a valid identifier\n",
				command->argv[i]);
			data->exit_status = 1;
		}
	}
}
