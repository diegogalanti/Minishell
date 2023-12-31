/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:58:29 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/28 18:01:55 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int single_command(t_data *data, t_command *command)
{
	int pid;
	int	child_exit_status;

	if (!command->argv[0])
		return (0);
	data->stdout_cpy = dup(STDOUT_FILENO);
	//close_fd(&data->stdout_cpy);
	if ((pid = fork()) < 0)
		return (printf("minishell: Error: fork process\n"), 0);
	if (pid == 0)
	{
		if (command->cmd == EXEC)
				execute_command(data, command);
		else
		{
			redirect(command);
			check_builtins(data, command, -2);
			exit_child(data, 0);
		}
		waitpid(pid, &child_exit_status, 0);
		get_child_exit_status(data, child_exit_status);
	}
	return (1);
}*/

int single_command(t_data *data, t_command *command)
{
	int pid;
	int	child_exit_status;

	if (!command->argv[0])
		return (0);
	if (command->cmd == EXEC)
	{
		if ((pid = fork()) < 0)
			return (printf("minishell: Error: fork process\n"), 0);
		if (pid == 0)
			execute_command(data, command);
		waitpid(pid, &child_exit_status, 0);
		get_child_exit_status(data, child_exit_status);
	}
	else
	{
	//	redirect(command);
		check_builtins(data, command, -2);
		undirect(command, data);
	}
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
	if (dir[i] == NULL)
		return (cmd);
	cmd = fs_strdup(data, test_cmd);
	free(test_cmd);
	test_cmd = NULL;
	return (cmd);
}


/* Question: Does reassigning command->argv[0] lead to a memory leak?
				(because only 1 element of argv is reassigned) */

void   execute_command(t_data *data, t_command *command)
{
	if (!command || !command->argv || !command->argv[0])
	{
		//dup2(data->stdout_cpy, STDOUT_FILENO);
		printf("%s: command not found\n", command->argv[0]);
		exit_child (data, 127);
	}
	if (access(command->argv[0], F_OK))
	{
		command->argv[0] = find_path(data, command->argv[0]);
		if (access(command->argv[0], F_OK))
		{
			//dup2(data->stdout_cpy, STDOUT_FILENO);
			printf("%s: command not found\n", command->argv[0]);
			exit_child (data, 127);
		}
	}
	//redirect(command);
	execve(command->argv[0], command->argv, NULL);
	undirect(command, data);
	printf("minishell: %s: %s\n", command->argv[0], strerror(errno));
	exit_child (data, errno);
}

/* execute: 1. checks if there are commands
			2. checks if there are redirections and sets them if necessary
			3. redirects
			4. if 1 command, executes it
			5. if 2 or more, it pipes
			*/
void    execute(t_data *data)
{
	if (!data->nb_cmds)
		return ;
	if (!check_redirections(data, set_redirections))
		return ;
	if (data->nb_cmds < 2 && data->commands)
		single_command(data, data->commands->content);
	else 
		pipe_commands(data);
}
