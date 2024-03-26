/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:08:19 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 16:18:05 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_cd: mimicks the behaviour of cd in bash
	it accepts absolute and relative path and .. and .
	as well as -- and -

	To Do (maybe, not sure if we have to):
	- use errno for error message
	- update OLDPWD so that cd - works properly*/

int	builtin_cd(t_data *data, t_command *command)
{
	int	ret;

	undirect(command, data);
	if (command->argv[1] == NULL)
		ret = chdir(ft_strtrim(find_var(data->env, "HOME"), "="));
	else if (command->argv[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	else if (!strncmp(command->argv[1], "-", ft_strlen(command->argv[1])))
		ret = chdir(ft_strtrim(find_var(data->env, "OLDPWD"), "="));
	else if (!strncmp(command->argv[1], "--", ft_strlen(command->argv[1]))
		|| !strncmp(command->argv[1], "~", ft_strlen(command->argv[1])))
		ret = chdir(ft_strtrim(find_var(data->env, "HOME"), "="));
	else
		ret = chdir(command->argv[1]);
	if (ret != 0)
	{
		print_error("cd", command->argv[1], strerror(errno));
		return (1);
	}
	return (0);
}
