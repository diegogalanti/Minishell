/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:58:29 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/17 15:58:32 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int single_command(t_data *data, t_command *command)
{
	int pid;

	if (!command->argv[0])
		return (1);
	if (command->cmd == EXEC)
	{
		if ((pid = fork()) < 0)
			return (printf("minishell: Error: fork process\n"), 1);
		if (pid == 0)
			execute_command(command);
		waitpid(pid, &data->exit_status, 0);
	}
	else
	  check_builtins(data, command);
	return (0);
}

void    execute_command(t_command *command)
{
 /*   int stdout_cpy;

	stdout_cpy = dup(STDOUT_FILENO);
	dup2(command->fd_in, STDIN_FILENO);
	dup2(command->fd_out, STDOUT_FILENO);
	close(command->fd_in);
	close(command->fd_out);*/
	execve(command->argv[0], command->argv, NULL);
  //  dup2(stdout_cpy, STDIN_FILENO);
  //  close(command->fd_out);
	printf("minishell: %s: %s\n", command->argv[0], strerror(errno));
}

int    execute(t_data *data)
{
	if (redirect(data) < 0)
		return (-1);
	if (data->nb_cmds < 2 && data->commands)
		single_command(data, data->commands->content);
	//		else 
	//			pipe(data);
}