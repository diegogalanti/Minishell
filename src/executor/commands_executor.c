/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:58:29 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/03/26 17:05:57 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	single_command(t_data *data, t_command *command)
{
	int	pid;
	int	child_exit_status;

	if (!command->argv[0])
		return (0);
	if (command->cmd == EXEC || command->cmd == NOT_FOUND)
	{
		pid = fork();
		if (pid < 0)
			return (ft_putstr_fd("minishell: Error: fork process\n", STDERR_FILENO), 1);
		if (pid == 0)
			execute_command(data, command);
		waitpid(pid, &child_exit_status, 0);
		get_child_exit_status(data, child_exit_status);
	}
	else
	{
		data->exit_status = check_builtins(data, command, -2);
	}
	undirect(command, data);
	return (0);
}

char	*find_path(t_data *data, char *cmd)
{
	char	**dir;
	char	*test_path;
	char	*test_cmd;
	int		i;

	dir = fs_split(data, find_var(data->env, "PATH"), ':');
	if (dir == NULL)
		return (cmd);
	i = -1;
	while (dir[++i])
	{
		test_path = ff_strjoin(dir[i], "/", data);
		test_cmd = ff_strjoin(test_path, cmd, data);
		test_path = NULL;
		if (!access(test_cmd, X_OK))
			break ;
		test_cmd = NULL;
	}
	if (dir[i] == NULL)
		return (cmd);
	cmd = fs_strdup(data, test_cmd);
	test_cmd = NULL;
	return (cmd);
}

/* Question: Does reassigning command->argv[0] lead to a memory leak?
				(because only 1 element of argv is reassigned) */

void	execute_command(t_data *data, t_command *command)
{
	if (!command || !command->argv || !command->argv[0]
		|| command->cmd == NOT_FOUND)
	{
		printf("%s: command not found\n", command->argv[0]);
		exit_child (data, command, 127);
	}
	if (access(command->argv[0], X_OK))
	{
		command->argv[0] = find_path(data, command->argv[0]);
		if (access(command->argv[0], X_OK))
		{
			print_error("", command->argv[0], "command not found");
			exit_child (data, command, 127);
		}
	}
	redirect(command, data);
//	if (find_mult_redir(command->argv))
//		trunc_mult_redir(command->argv);
	execve(command->argv[0], command->argv, data->env);
	undirect(command, data);
	print_error("", command->argv[0], strerror(errno));
	exit_child (data, command, errno);
}

/* execute: 1. checks if there are commands
			2. checks if there are redirections and sets them if necessary
			3. redirects
			4. if 1 command, executes it
			5. if 2 or more, it pipes
			*/
void	execute(t_data *data)
{
	if (!data->nb_cmds)
		return ;
	if (data->nb_cmds < 2 && data->commands)
	{
		if (!set_redirections(data->commands->content))
		{
			reset_data(data);
			data->exit_status = 1;
			return ;
		}
		single_command(data, data->commands->content);
	}
	else
	{
		check_redirections(data, set_redirections);
		pipe_commands(data);
	}
}
