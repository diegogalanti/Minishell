/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:46:01 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/22 11:36:29 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_echo: mimicks "echo" in bash
	writes all the characters after "echo" to stdout and prints a newline
	if there are more than one space, the superflue spaces are omitted
	with "echo -n" the newline at the end is not printed */

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
	*/

void	check_builtins(t_data *data, t_command *command, int i)
{
	redirect(command, data);
	if (find_mult_redir(command->argv))
		trunc_mult_redir(command->argv);
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
