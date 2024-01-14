/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:46:01 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/14 10:08:40 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_echo: mimicks "echo" in bash
	writes all the characters after "echo" to stdout and prints a newline
	if there are more than one space, the superflue spaces are omitted
	with "echo -n" the newline at the end is not printed
	TO DO: ; -> after ";" is interpreted as command (Do we have to implement that?)
	*/

void	builtin_echo(t_command *command)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (command->argv[i] && !strncmp(command->argv[i], "-n", 2))
	{
		n = 1;
		i++;
	}
	while (command->argv[i] != NULL)
	{
		if (i > 1 + n)
			printf("%c", 32);
		printf("%s", command->argv[i]);
		i++;
	}
	if (n == 0)
		printf("\n");
}

/* builtin_pwd: mimicks the behaviour of pwd in bash
	With the help of getcwd() the absolute path of the current working
	directory is saved in a buffer.
	This string is the printed on the screen.
	Questions: What if getcwd does not behave as it should?
		If it returns NULL for example?
		How to we deal with redirections? this could have an impact on
		whether to use printf or ft_putstr
		Is buffer size good?*/

void	builtin_pwd(void)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, 1000);
	printf("%s\n", buf);
	free(buf);
	buf = NULL;
}

/* builtin_cd: mimicks the behaviour of cd in bash
	it accepts absolute and relative path and .. and .
	as well as -- and -

	To Do (maybe, not sure if we have to):
	- use errno for error message
	- update OLDPWD so that cd - works properly*/

void	builtin_cd(t_data *data, t_command *command)
{
	int	ret;

	undirect(command, data);
	if (command->argv[1] == NULL)
	{
		ret = chdir(ft_strtrim(find_var(data->env, "HOME"), "="));
		return ;
	}
	if (!strncmp(command->argv[1], "-", ft_strlen(command->argv[1])))
		ret = chdir(ft_strtrim(find_var(data->env, "OLDPWD"), "="));
	else if (!strncmp(command->argv[1], "--", ft_strlen(command->argv[1])))
		ret = chdir(ft_strtrim(find_var(data->env, "HOME"), "="));
	else
		ret = chdir(command->argv[1]);
	if (ret == 0)
		return ;
	else
		printf("minishell: cd: path not found\n");
}

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
	This checker assumes that the parser already took hand of special characters (e.g. brackets)
	and changed variables (e.g. $USER) to the string they refer to (e.g. username).
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

/* TO DO: change variables*/
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
	if no variable is given, it just returns
	TO DO: Does not work if variable name is given only*/

void	builtin_unset(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	while (command->argv[++i] != NULL)
		data->env = del_var(data, data->env, command->argv[i]);
}

/* builtin_env: prints the environment variables
	if env is followed by a valid new variable, the new var is printed, too (but not added to env)
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

/* builtin_exit: exits the shell
		if arguments are given it does not exit but prints error message*/

void	builtin_exit(t_data *data, t_command *command, int i)
{
	if (data->pipe && i >= 0)
	{
		printf("exit\n");
		if (command->argv[1] && data->pid[i] == 0)
		{
			dup2(data->stdout_cpy, STDOUT_FILENO);
			close(data->stdout_cpy);
			printf("minishell: exit: too many arguments\n");
			exit_child(data, 1);
		}
		else
			exit_child(data, 0);
	}
	undirect(command, data);
	printf("exit\n");
	if (command->argv[1])
		printf("minishell: exit: too many arguments\n");
	else
		free_exit(data);
}

/* builtins: This function checks if one of the builtins is requested.
	If it is, a function which handles the specific builtin is called.
	Otherwise, this functions returns, without doing anything.
	TO DO: Now all the functions write to stdout. As soon as redirections are done,
	change 1 to fd.*/

void	check_builtins(t_data *data, t_command *command, int i)
{
	if (command->cmd == ECHO)
		builtin_echo(command);
	else if (command->cmd == CD)
		builtin_cd(data, command);
	else if (command->cmd == PWD)
		builtin_pwd();
	else if (command->cmd == EXPORT)
		builtin_export(data, command);
	else if (command->cmd == UNSET)
		builtin_unset(data, command);
	else if (command->cmd == ENV)
		builtin_env(data, command);
	else if (command->cmd == EXIT)
		builtin_exit(data, command, i);
}
