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
		return (0);
	if (command->cmd == EXEC)
	{
		if ((pid = fork()) < 0)
			return (printf("minishell: Error: fork process\n"), 0);
		if (pid == 0)
			execute_command(data, command);
		waitpid(pid, &data->exit_status, 0);
	}
	else
	  check_builtins(data, command);
	return (1);
}

char	*find_path(t_data *data, char *cmd)
{
	char	**dir;
	char	*test_path;
	char	*test_cmd;
	int		i;

	dir = fs_split(data, find_var(data->env, "PATH"), ':');
	if (dir == NULL)
		return (NULL);
	i = -1;
	while (dir[++i])
	{
		test_path = ft_strjoin(dir[i], "/");
		test_cmd = ft_strjoin(test_path, cmd);
		free(test_path);
		test_path = NULL;
		if (!access(test_cmd, F_OK))
			break ;
		free(test_cmd);
		test_cmd = NULL;

	}
	cmd = fs_strdup(data, test_cmd);
	free(test_cmd);
	test_cmd = NULL;
	return (cmd);
}

/* Question: Does reassigning command->argv[0] lead to a memory leak?
				(because only 1 element of argv is reassigned) */

int   execute_command(t_data *data, t_command *command)
{
	int stdout_cpy;

	if (!command || !command->argv || !command->argv[0])
	{
		close(command->fd_in);
		close(command->fd_out);
		return (127);
	}
	if (access(command->argv[0], F_OK))
	{
		command->argv[0] = find_path(data, command->argv[0]);
		if (!command->argv[0])
			return (printf("Minishell: Could not find command\n"), 127);
	}
	stdout_cpy = dup(STDOUT_FILENO);
	dup2(command->fd_in, STDIN_FILENO);
	dup2(command->fd_out, STDOUT_FILENO);
	close(command->fd_in);
	close(command->fd_out);
	execve(command->argv[0], command->argv, NULL);
	dup2(stdout_cpy, STDIN_FILENO);
	close(command->fd_out);
	printf("minishell: %s: %s\n", command->argv[0], strerror(errno));
	exit (errno);
}

void    execute(t_data *data)
{
	//if (redirect(data) < 0)
	//	return (-1);
	if (data->nb_cmds < 2 && data->commands)
		single_command(data, data->commands->content);
	else 
		pipe_commands(data);
}