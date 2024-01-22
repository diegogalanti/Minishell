/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:38:52 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/22 11:40:41 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(char **env)
{
	int	i;
	int	j;

	i = -1;
	while (env[++i] != NULL)
	{
		printf("declare -x ");
		j = -1;
		while (env[i][++j] != '=')
			printf("%c", env[i][j]);
		printf("%c", env[i][j++]);
		printf("%c", 34);
		while (env[i][++j] != '\0')
			printf("%c", env[i][j]);
		printf("%c\n", 34);
	}
}

/* check_export_var: checks if variable that user wants to add to env is valid
	This checker assumes that the parser already took hand of special characters 
		(e.g. brackets) and changed variables (e.g. $USER) to the string 
		they refer to (e.g. username).
	Thus all ascii characters from 33 to 126 are allowed.
	At least one '=' is needed for the variable to be valid and added to env.
	If it is not valid, check_export_var returns 0 and no variable is added. */

int	check_export_var(char *str)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			y = 1;
		if (33 <= str[i] && str[i] < 127)
			i++;
		else
			return (0);
	}
	if (!y)
		return (0);
	return (1);
}

/* builtin_export: 
    "export var_name=var_value" creates a new variable
        or changes the value if variable already exists */
        
void	builtin_export(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	if (command->argv[1] == NULL && data->env)
	{
		print_export(data->env);
		return ;
	}
	while (command->argv[++i] != NULL)
	{
		if (check_export_var(command->argv[i]))
			data->env = add_mod_var(data, data->env, command->argv[i]);
	}
}

/* builtin_unset: deletes a or several environment variables
	if no variable is given, it just returns*/

void	builtin_unset(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	while (command->argv[++i] != NULL)
		data->env = del_var(data, data->env, command->argv[i]);
}

/* builtin_env: prints the environment variables
	if env is followed by a valid new variable, 
		the new var is printed, too (but not added to env)
	if env is followed by a not valid new variable, it prints an error message*/

void	builtin_env(t_data *data, t_command *command)
{
	int	i;

	if (!data->env)
	{
		printf("minishell: no environment variable available\n");
		return ;
	}
	i = 0;
	while (command->argv[++i] != NULL)
	{
		if (!check_export_var(command->argv[i]))
		{
			printf("env: '%s': No such file or directory\n", command->argv[i]);
			return ;
		}
	}
	i = -1;
	while (data->env[++i] != NULL)
		printf("%s\n", data->env[i]);
	i = 0;
	while (command->argv[++i] != NULL)
		printf("%s\n", command->argv[i]);
}
