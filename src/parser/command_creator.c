/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_creator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:52:07 by digallar          #+#    #+#             */
/*   Updated: 2024/01/28 17:53:34 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_type(t_command *command)
{
	if (!command->argv[0])
		command->cmd = NONE;
	if (ft_strlen(command->argv[0]) == 0)
		command->cmd = NOT_FOUND;
	else if (!strncmp(command->argv[0], "echo", ft_strlen(command->argv[0])))
		command->cmd = ECHO;
	else if (!strncmp(command->argv[0], "cd ", ft_strlen(command->argv[0])))
		command->cmd = CD;
	else if (!strncmp(command->argv[0], "pwd", ft_strlen(command->argv[0])))
		command->cmd = PWD;
	else if (!strncmp(command->argv[0], "env", ft_strlen(command->argv[0])))
		command->cmd = ENV;
	else if (!strncmp(command->argv[0], "exit", ft_strlen(command->argv[0])))
		command->cmd = EXIT;
	else if (!strncmp(command->argv[0], "export", ft_strlen(command->argv[0])))
		command->cmd = EXPORT;
	else if (!strncmp(command->argv[0], "unset", ft_strlen(command->argv[0])))
		command->cmd = UNSET;
	else
		command->cmd = EXEC;
}

/* Tatiana: I added data->nb_cmds to count the number of commands here.
	Thought it was a good place because the commands are created here.*/

void	create_command(t_data *data, int start, int end, int next_c)
{
	t_command	*command ;

	command = safe_malloc(data, sizeof(*command));
	if (!data->commands)
		data->commands = ft_lstnew(command);
	else
		ft_lstadd_back(&data->commands, ft_lstnew(command));
	command->env = data->env;
	command->free_list = data->free_list;
	command->cmd_input = fs_substr(data->user_input,
			start, end - start, command);
	command->stdin = 0;
	command->stdout = 0;
	command->limiter = 0;
	command->exit_status = data->exit_status;
	command->found_limiter = 0;
	command->append_mode = -1;
	//ft_printf("Command input = [%s]\n", command->cmd_input);
	expand_vars(command);
	data->exit_status = build_redirections(command, next_c);
	if (data->exit_status != 0)
		return ;
	alloc_argv(command);
	build_argv(command);
	add_type(command);
	data->nb_cmds++;
}
