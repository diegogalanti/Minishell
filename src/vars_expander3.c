/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_expander3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:02:09 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 08:49:03 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(t_command *command, int start, int end, int iq)
{
	char	*env_var;
	char	*env_var_value;
	char	*tmp_cmd;
	char	old_size;

	tmp_cmd = command->cmd_input;
	old_size = ft_strlen(command->cmd_input);
	env_var = fs_substr(command->cmd_input, start, (end - start) + 1, command);
	env_var_value = find_var(command->env, &env_var[1]);
	if (!env_var_value)
	{
		env_var_value = command_safe_malloc(command, 1);
		env_var_value[0] = 0;
	}
	if (!iq)
	{
		env_var_value = fs_strjoin("\"", env_var_value, command);
		env_var_value = fs_strjoin(env_var_value, "\"", command);
	}
	tmp_cmd = fs_strjoin(fs_substr(command->cmd_input, 0, start, command),
			env_var_value, command);
	command->cmd_input = fs_strjoin(tmp_cmd, fs_substr(command->cmd_input,
				end + 1, ft_strlen(command->cmd_input) - end, command),
			command);
	return (ft_strlen(command->cmd_input) - old_size);
}

int	expand_exit_status(t_command *comm, int start, int end, int iq)
{
	char	*env_var_value;
	char	*tmp_cmd;
	char	old_size;

	tmp_cmd = comm->cmd_input;
	old_size = ft_strlen(comm->cmd_input);
	env_var_value = ft_itoa(comm->exit_status);
	if (!iq)
	{
		env_var_value = fs_strjoin("\"", env_var_value, comm);
		env_var_value = fs_strjoin(env_var_value, "\"", comm);
	}
	tmp_cmd = fs_strjoin(fs_substr(comm->cmd_input, 0, start, comm),
			env_var_value, comm);
	comm->cmd_input = fs_strjoin(tmp_cmd, fs_substr(comm->cmd_input,
				end + 1, ft_strlen(comm->cmd_input) - end, comm), comm);
	return (ft_strlen(comm->cmd_input) - old_size);
}
