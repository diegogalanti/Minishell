/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:53:29 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 09:52:21 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_space_ab(t_parse_status current)
{
	if (current == FOUND_SQOT_WFC)
		return (FOUND_SQOT);
	else if (current == FOUND_DQOT_WFC)
		return (FOUND_DQOT);
	return (current);
}

static void	check_final_ab(t_command *command, int start_i, int i, int pos)
{
	command->argv[pos] = fs_substr(command->cmd_input, start_i,
			i - start_i, command);
	remove_quotes(command->argv[pos]);
	ft_printf("Argument = [%s]\n", command->argv[pos]);
}

static void	start_vars(t_parse_status *status, int *start_i, int *i, int *pos)
{
	*i = -1;
	*pos = 0;
	*start_i = 0;
	*status = WAITING_FOR_CHAR;
}

void	build_argv(t_command *c)
{
	int				i;
	int				pos;
	int				start_i;
	t_parse_status	status;

	start_vars(&status, &start_i, &i, &pos);
	while (c->cmd_input[++i])
	{
		if (ft_isspace(c->cmd_input[i]))
		{
			status = check_space_ab(status);
			if (status == WAITING_FOR_SPACE)
			{
				c->argv[pos] = fs_substr(c->cmd_input, start_i, i - start_i, c);
				ft_printf("Argument = [%s]\n", c->argv[pos]);
				remove_quotes(c->argv[pos]);
				start_i = i + 1;
				pos++;
				status = WAITING_FOR_CHAR;
			}
		}
		else
			status = check_nonspace_ab(status, &start_i, i, c->cmd_input[i]);
	}
	if (status == WAITING_FOR_SPACE)
		check_final_ab(c, start_i, i, pos);
}
