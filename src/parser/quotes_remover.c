/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:57:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 10:26:37 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parse_status	check_dquotes_qr(t_parse_status current,
	char *arg, int *i)
{
	if (current == WAITING_FOR_QOT)
	{
		ft_memmove(arg + *i, arg + *i + 1, ft_strlen(arg) - *i);
		(*i)--;
		return (FOUND_DQOT);
	}
	else if (current == FOUND_DQOT)
	{
		ft_memmove(arg + *i, arg + *i + 1, ft_strlen(arg) - *i);
		(*i)--;
		return (WAITING_FOR_QOT);
	}
	return (current);
}

static t_parse_status	check_squotes_qr(t_parse_status current,
	char *arg, int *i)
{
	if (current == WAITING_FOR_QOT)
	{
		ft_memmove(arg + *i, arg + *i + 1, ft_strlen(arg) - *i);
		(*i)--;
		return (FOUND_SQOT);
	}
	else if (current == FOUND_SQOT)
	{
		ft_memmove(arg + *i, arg + *i + 1, ft_strlen(arg) - *i);
		(*i)--;
		return (WAITING_FOR_QOT);
	}
	return (current);
}

void	remove_quotes(char *arg)
{
	int				i;
	t_parse_status	status;

	i = -1;
	status = WAITING_FOR_QOT;
	while (arg[++i])
	{
		if (arg[i] == '\"')
			status = check_dquotes_qr(status, arg, &i);
		else if (arg[i] == '\'')
			status = check_squotes_qr(status, arg, &i);
	}
}
