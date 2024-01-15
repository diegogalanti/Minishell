/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_builder3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:44:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 14:50:09 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_limiter(t_command *cmd, int start, int end)
{
	int	i;
	int	old_size;

	i = start;
	old_size = ft_strlen(cmd->cmd_input);
	while (1)
	{
		if (cmd->cmd_input[i] != '<' && cmd->cmd_input[i] != '>'
			&& !ft_isspace(cmd->cmd_input[i]))
			break ;
		i++;
	}
	cmd->limiter = fs_substr(cmd->cmd_input, i, end - i, cmd);
	cmd->cmd_input = fs_strjoin(fs_substr(cmd->cmd_input, 0, start, cmd),
			fs_substr(cmd->cmd_input, end,
				ft_strlen(cmd->cmd_input) - end, cmd), cmd);
	cmd->found_limiter = 0;
	remove_quotes(cmd->limiter);
	ft_printf("Limiter = [%s]\n", cmd->limiter);
	return (ft_strlen(cmd->cmd_input) - old_size);
}

static int	get_redirection_type(t_command *command, int start, int *i)
{
	int	type;

	*i = start;
	type = -1;
	while (1)
	{
		if (command->cmd_input[*i] == '<')
			type = 0;
		else if (command->cmd_input[*i] == '>')
			type = 1;
		else if (!ft_isspace(command->cmd_input[*i]))
			break ;
		(*i)++;
	}
	return (type);
}

int	create_redirection(t_command *cmd, int start, int end)
{
	int	i;
	int	type;
	int	old_size;

	if (cmd->found_limiter == 1)
		return (create_limiter(cmd, start, end));
	type = get_redirection_type(cmd, start, &i);
	old_size = ft_strlen(cmd->cmd_input);
	if (type == 0)
	{
		cmd->stdin = fs_substr(cmd->cmd_input, i, end - i, cmd);
		remove_quotes(cmd->stdin);
		ft_printf("Redirection input = [%s]\n", cmd->stdin);
	}
	else if (type == 1)
	{
		cmd->stdout = fs_substr(cmd->cmd_input, i, end - i, cmd);
		remove_quotes(cmd->stdout);
		ft_printf("Redirection output = [%s]\n", cmd->stdout);
	}
	cmd->cmd_input = fs_strjoin(fs_substr(cmd->cmd_input, 0, start, cmd),
			fs_substr(cmd->cmd_input, end,
				ft_strlen(cmd->cmd_input) - end, cmd), cmd);
	return (ft_strlen(cmd->cmd_input) - old_size);
}

void	print_error_pipe(void)
{
	ft_printf("-minishell: syntax error near unexpected token `|'\n");
}

void	print_error_nl(void)
{
	ft_printf("-minishell: syntax error near unexpected token `newline'\n");
}
