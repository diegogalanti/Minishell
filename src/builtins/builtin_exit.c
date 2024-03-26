/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:52:06 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 16:24:15 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exit_n_input(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	while ((str[i] == 32) || ((str[i] >= 9) && (str[i] <= 13)))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (ft_isdigit(str[i]))
		i++;
	while ((str[i] == 32) || ((str[i] >= 9) && (str[i] <= 13)))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	get_exit_status(t_data *data, char *input)
{
	if (!check_exit_n_input(input))
	{
		print_error("exit", input, "numeric argument required");
		data->exit_status = 2;
		free_exit(data);
	}
	return (ft_atoi(input));
}

/* builtin_exit: exits the shell
		if arguments are given it does not exit but prints error message*/

int	builtin_exit(t_data *data, t_command *command, int i)
{
	if (data->pipe && i >= 0)
	{
		if (command->argv[1] && data->pid[i] == 0)
			exit_child(data, command, 1);
		else
			exit_child(data, command, 0);
	}
	undirect(command, data);
	printf("exit\n");
	if (command->argv[1])
	{
		data->exit_status = get_exit_status(data, command->argv[1]);
		if (command->argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			data->exit_status = 1;
			return (1);
		}
	}
	free_exit(data);
	return (0);
}
