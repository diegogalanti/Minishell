/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:46:01 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/13 13:46:05 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_echo: mimicks "echo" in bash
	writes all the characters after "echo" to stdout and prints a newline
	if there are more than one space, the superflue spaces are omitted
	with "echo -n" the newline at the end is not printed
	TO DO: "" -> should not be printed
			; -> after ";" is interpreted as command (Do we have to implement that?)
		Question: are "" handled here or in the parser?
	*/

void	builtin_echo(t_data *data)
{
	int	i;
	char	**str;
	int	n;

	str = ft_split(data->user_input + 4, 32);
	i = 0;
	n = 0;
	if (!strncmp(str[i], "-n", 2))
	{
		n = 1;
		i++;
	}
	while (str[i])
	{
		if (i != 0)
			ft_putchar_fd(32, 1);
		ft_putstr_fd(ft_strtrim(str[i], "  "), 1);
		i++;
	}
	if (n == 0)
		ft_putchar_fd('\n', 1);
	free_str(str);
	str = NULL;
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

void	builtin_pwd()
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, 1000);
	printf("%s\n", buf);
	free(buf);
	buf = NULL;
}

void	builtin_cd(char *str)
{
	if (chdir(str) == 0)
		return ;
}

/* builtins: This function checks if one of the builtins is requested.
	This is checked with an easy strcmp function.
	If it is, a function which handles the specific builtin is called.
	Otherwise, this functions returns, without doing anything. */

void    builtins(t_data *data)
{
	if (!ft_strncmp(data->user_input, "exit", ft_strlen(data->user_input)))
		free_exit(&data);
	else if (!ft_strncmp(data->user_input, "echo ", 5))
		builtin_echo(data);
	else if (!ft_strncmp(data->user_input, "pwd", ft_strlen(data->user_input)))
		builtin_pwd();
	else if(!ft_strncmp(data->user_input, "cd ", 3))
		builtin_cd(data->user_input + 3);
}
