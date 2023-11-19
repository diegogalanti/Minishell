/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2023/11/19 20:15:07 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\n' || c == '\t' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r');
}

void	skip_spaces(t_command *command, int *i)
{
	while(ft_isspace(command->cmd_input[*i]))
	{
		(*i)++;
	}
}

void alloc_argv(t_command *command)
{
	int	i;
	int size;
	t_parse_status status;

	i = -1;
	size = 0;
	status = WAITING_FOR_CHAR;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\"')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_SPACE;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_DQOT;
		}
		else if (command->cmd_input[i] == '\'')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_SQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
				status = FOUND_SQOT;
		}
		else if (ft_isspace(command->cmd_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_SPACE)
			{
				status = WAITING_FOR_CHAR;
				size++;
			}
		}
		else
		{
			if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_SPACE;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
		}
	}
	if (status == WAITING_FOR_SPACE)
		size++;
	printf("Command has %i arguments.\n", size);
	command->argv = command_safe_malloc(command, 8 * size);
}

void	create_command(t_data *data, int start, int end)
{
	t_command *command = safe_malloc(data, sizeof(t_command));
	if (!data->commands)
		data->commands = ft_lstnew(command);
	else
		ft_lstadd_back(&data->commands, ft_lstnew(command));
	command->cmd_input = ft_substr(data->user_input, start, end - start);
	printf("Command input = [%s]\n", command->cmd_input);
	alloc_argv(command);

}

//I HAVE TO HANDLE COMPLETELY EMPTY COMMANDS, IT SHOULD NOT TRY TO RUN OTHER COMMAND.EG. "echo abc| |echo cde" should just return 258 and print "-bash: syntax error near unexpected token `|'"
int	split_commands(t_data *data)
{
	int	i;
	t_parse_status status;
	int start_i;

	i = -1;
	start_i = 0;
	status = WAITING_FOR_CHAR;
	while (data->user_input[++i])
	{
		if (data->user_input[i] == '\"')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_DQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_DQOT)
				status = WAITING_FOR_PIPE;
			else if (status == WAITING_FOR_PIPE)
				status = FOUND_DQOT;
		}
		else if (data->user_input[i] == '\'')
		{
			if (status == WAITING_FOR_CHAR)
				status = FOUND_SQOT_WFC;
			else if (status == FOUND_SQOT_WFC)
				status = WAITING_FOR_CHAR;
			else if (status == FOUND_SQOT)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
				status = FOUND_SQOT;
		}
		else if (data->user_input[i] == '|')
		{
			if (status == WAITING_FOR_CHAR)
			{
				printf("-minishell: syntax error near unexpected token `|'\n");
				return (258);
			}
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
			{
				create_command(data, start_i, i);
				start_i = i + 1;
				data->nb_cmds++;
				status = WAITING_FOR_CHAR;
			}
		}
		else if (ft_isspace(data->user_input[i]))
		{
			if (status == WAITING_FOR_CHAR)
				continue;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_SQOT)
				continue;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
			else if (status == FOUND_DQOT)
				continue;
			else if (status == WAITING_FOR_PIPE)
				continue;
		}
		else
		{
			if (status == WAITING_FOR_CHAR)
				status = WAITING_FOR_PIPE;
			else if (status == FOUND_SQOT_WFC)
				status = FOUND_SQOT;
			else if (status == FOUND_DQOT_WFC)
				status = FOUND_DQOT;
		}
	}
	if (status == WAITING_FOR_PIPE)
	{
		create_command(data, start_i, i);
		data->nb_cmds++;
	}
	return (0);
}

void	parse_input(t_data *data)
{
	split_commands(data);
	
	//code do split user_input into cmd_input
	//code to break command_input into tokens
	/* one block for each command */
	//first command
	data->nb_cmds = 1;
	t_command *command1 = safe_malloc(data, sizeof(t_command));
	if (!strncmp(data->user_input, "echo", 4))
		command1->cmd = ECHO;
	else if (!strncmp(data->user_input, "cd ", 3))
		command1->cmd = CD;
	else if (!strncmp(data->user_input, "pwd", ft_strlen(data->user_input)))
		command1->cmd = PWD;
	else if (!strncmp(data->user_input, "env", 3))
		command1->cmd = ENV;
	else if (!strncmp(data->user_input, "exit", 4))
		command1->cmd = EXIT;
	else if (!strncmp(data->user_input, "export", 6))
		command1->cmd = EXPORT;
	else if (!strncmp(data->user_input, "unset", 5))
		command1->cmd = UNSET;
	command1->argv = fs_split(data, data->user_input, ' ');
	/* one block for each command */
	//first command
	//	t_command *command1 = safe_malloc(data, sizeof(t_command));
	//	command1->cmd = ECHO;
	//	command1->argv = safe_malloc(data, 8 * 4); /* 2 is the number of arguments for the command */
	//		command1->argv[0] = "echo";
	//		command1->argv[1] = "abc";
	//		command1->argv[2] = "$USER";
	//		command1->argv[3] = NULL;
	//	command1->i_redirect = "/folde/input.txt"; /* set to 0 to use stdin */
	//	command1->o_redirect = "/folde/output.txt"; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//	command1->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	
	//second command
	//t_command *command2 = safe_malloc(data, sizeof(t_command));
	//	command2->cmd = CD;
	//	command2->argv = safe_malloc(data, 8 * 3); /* 2 is the number of arguments for the command */
	//		command2->argv[0] = "cd";
	//		command2->argv[1] = "/home/tstahlhu/42";
	//		command2->argv[2] = NULL;
	//	command2->i_redirect = 0; /* set to 0 to use stdin */
	//	command2->o_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//	command2->a_redirect = 0; /* set to 0 to use stdout. o_redirect and a_redirect cannot be used together. */
	//for the first command
	data->commands = ft_lstnew(command1);

	//for every additional command
	//ft_lstadd_back(&data->commands, ft_lstnew(command2));
	//ft_lstadd_back(&data->commands, ft_lstnew(command3));
}