/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:38:52 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/27 17:02:22 by tstahlhu         ###   ########.fr       */
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
		if (!is_var(data->shvar, var))
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

/* check_export_var: checks if variable that user wants to add to env is valid
	It first checks if the strings are empty and if the variable name is valid 
		(see env_var_name_check(char *str) for more info).
	This checker assumes that the parser already took hand of special characters 
		(e.g. brackets, quotes) and changed variables (e.g. $USER) to the string 
		they refer to (e.g. username).
	Thus all ascii characters from 33 to 126 are allowed for the variables value.
	If it is not valid, check_export_var returns 0 and no variable is added.
	If it is valid and contains at least one '=', var will be added to env and 0 is returned.
	If it is valid and does not contain a '=', var will be added to shvar and 0 is returned.
	 */

/*int	check_export_add(char *str, t_data *data)
{
	int 	i;
	int		y;

	i = 0;
	y = 0;
	if (!str || str[i] =='\0' || env_var_name_check(str))
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			y = 1;
		if (33 <= str[i] && str[i] < 127)
			i++;
		else
			return (1);
	}
	if (!y)
		data->shvar = add_mod_var(data, data->shvar, str);
	else
		data->env = add_mod_var(data, data->env, str);
	return (0);
}*/

/* builtin_export: 
	"export var_name=var_value" creates a new variable
		or changes the value if variable already exists 
	*/		
/*void	builtin_export(t_data *data, t_command *command)
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
		if (check_export_add(command->argv[i], data))
		{
			printf("minishell: export: '%s': not a valid identifier\n",
				command->argv[i]);
			data->exit_status = 1;
		}
	}
}*/

/*int	check_export_var(char *str)
{
	int 	i;
	int		equal_sign;
	int		add;

	i = 0;
	equal_sign = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			equal_sign = 1;
		if (str[i] == '+' && str[i + 1] == '=')
			add = 1;
		if (33 <= str[i] && str[i] < 127)
			i++;
		else
			return (0);
	}
	if (add)
		return (2);
	if (equal_sign)
		return (1);
	return (0);
}*/

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

/* builtin_unset: deletes a or several environment variables
	if no variable is given, it just returns*/

void	builtin_unset(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	while (command->argv[++i] != NULL)
	{
		data->env = del_var(data, data->env, command->argv[i]);
		data->shvar = del_var(data, data->shvar, command->argv[i]);
	}
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
		if (!has_equal_sign(command->argv[i]))
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
