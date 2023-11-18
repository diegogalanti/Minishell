/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:35:12 by digallar          #+#    #+#             */
/*   Updated: 2023/11/19 20:12:11 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\n' || c == '\t' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r');
}

void alloc_argv(t_command *command)
{
	int	i;
	int	qot;
	int size;

	qot = 0;
	i = -1;
	size = 1;
	while(ft_isspace(command->cmd_input[++i]))
	{
	}
	i--;
	while (command->cmd_input[++i])
	{
		if (command->cmd_input[i] == '\"' && !qot)
			qot = 1;
		else if (command->cmd_input[i] == '\"' && qot == 1)
			qot = 0;
		else if (command->cmd_input[i] == '\'' && !qot)
			qot = 2;
		else if (command->cmd_input[i] == '\'' && qot == 2)
			qot = 0;
		else if (ft_isspace(command->cmd_input[i]) && !qot)
		{
			while(ft_isspace(command->cmd_input[i + 1]))
			{
				i++;
			}
			size++;
			//se for espaco e nada depois? tem que checar
		}
	}
}

void	create_command(t_data *data, int start, int end)
{
	t_command *command = safe_malloc(data, sizeof(t_command));
	if (!data->commands)
		data->commands = ft_lstnew(command);
	else
		ft_lstadd_back(&data->commands, ft_lstnew(command));
	command->cmd_input = ft_substr(data->user_input, start, end - start);
	alloc_argv(command);

}

void	split_commands(t_data *data)
{
	int	i;
	int	qot;
	int start_i;

	qot = 0;
	i = -1;
	start_i = 0;
	while (data->user_input[++i])
	{
		if (data->user_input[i] == '\"' && !qot)
			qot = 1;
		else if (data->user_input[i] == '\"' && qot == 1)
			qot = 0;
		else if (data->user_input[i] == '\'' && !qot)
			qot = 2;
		else if (data->user_input[i] == '\'' && qot == 2)
			qot = 0;
		else if ((data->user_input[i] == '|' && !qot) || data->user_input[i + 1] == 0)
		{
			create_command(data, start_i, i);
			start_i = i + 1;
		}
	}
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