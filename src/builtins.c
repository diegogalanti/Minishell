/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:46:01 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/28 15:21:00 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_echo: mimicks "echo" in bash
	writes all the characters after "echo" to stdout and prints a newline
	if there are more than one space, the superflue spaces are omitted
	with "echo -n" the newline at the end is not printed */

int	builtin_echo(t_command *command)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	while (command->argv[i] && !strncmp(command->argv[i], "-n", ft_strlen(command->argv[i])))
	{
		n++;
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
    return (0);
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

/* builtins: This function checks if one of the builtins is requested.
	If it is, a function which handles the specific builtin is called.
	Otherwise, this functions returns, without doing anything.
	*/

int	check_builtins(t_data *data, t_command *command, int i)
{
    int status;

	redirect(command, data);
	if (find_mult_redir(command->argv))
		trunc_mult_redir(command->argv);
    status = 0;
	if (command->cmd == ECHO)
		status = builtin_echo(command);
	else if (command->cmd == CD)
		status = builtin_cd(data, command);
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
    return (status);
}
