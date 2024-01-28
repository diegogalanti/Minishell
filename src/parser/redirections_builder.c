/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:44:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/28 16:21:09 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_bigger_rb(t_parse_status current, int *start_i,
	int *i, t_command *command)
{
	if (current == WAITING_FOR_RED)
	{
		*start_i = *i;
		command->append_mode = 0;
		return (FOUND_ORED);
	}
	else if (current == FOUND_IRED)
	{
		return (ft_printf("-minishell: syntax error near unexpected "
				"token `%c'\n", command->cmd_input[*i]), ABORT);
	}
	else if (current == FOUND_ORED)
	{
		command->append_mode = 1;
		return (WFC);
	}
	else if (current == WFC)
		return (ft_printf("-minishell: syntax error near unexpected "
				"token `%c'\n", command->cmd_input[*i]), ABORT);
	else if (current == WAITING_FOR_SPACE)
	{
		*i += create_redirection(command, *start_i, *i);
		return (FOUND_ORED);
	}
	return (current);
}

static t_parse_status	check_smaller_rb(t_parse_status current,
	int *start_i, int *i, t_command *command)
{
	if (current == WAITING_FOR_RED)
	{
		*start_i = *i;
		return (FOUND_IRED);
	}
	else if (current == FOUND_ORED)
		return (ft_printf("-minishell: syntax error near unexpected "
				"token `%c'\n", command->cmd_input[*i]), ABORT);
	else if (current == FOUND_IRED)
	{
		command->found_limiter = 1;
		return (WFC);
	}
	else if (current == WFC)
		return (ft_printf("-minishell: syntax error near unexpected "
				"token `%c'\n", command->cmd_input[*i]), ABORT);
	else if (current == WAITING_FOR_SPACE)
	{
		*i += create_redirection(command, *start_i, *i);
		return (FOUND_IRED);
	}
	return (current);
}

static void	start_vars(int *i, int *start_i, t_parse_status *status)
{
	*i = -1;
	*start_i = 0;
	*status = WAITING_FOR_RED;
}

int	build_redirections(t_command *command, int x)
{
	int				i;
	int				start_i;
	t_parse_status	st;

	start_vars(&i, &start_i, &st);
	while (command->cmd_input[++i] && st != ABORT)
	{
		if (ft_isspace(command->cmd_input[i]))
			st = check_space_rb(st, start_i, &i, command);
		else if (command->cmd_input[i] == '>')
			st = check_bigger_rb(st, &start_i, &i, command);
		else if (command->cmd_input[i] == '<')
			st = check_smaller_rb(st, &start_i, &i, command);
		else
			st = check_quotesother_rb(command->cmd_input[i], st);
	}
	if (st == ABORT)
		return (258);
	if ((st == FOUND_ORED || st == FOUND_IRED || st == WFC) && x == 0)
		return (print_error_pipe(), 258);
	else if ((st == FOUND_ORED || st == FOUND_IRED || st == WFC) && x == 1)
		return (print_error_nl(), 258);
	else if (st == WAITING_FOR_SPACE)
		create_redirection(command, start_i, i);
	return (0);
}
