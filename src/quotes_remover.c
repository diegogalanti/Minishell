/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:57:32 by digallar          #+#    #+#             */
/*   Updated: 2024/01/15 08:57:45 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(char *arg)
{
	int				i;
	t_parse_status	status;

	i = -1;
	status = WAITING_FOR_QOT;
	while (arg[++i])
	{
		if (arg[i] == '\"')
		{
			if (status == WAITING_FOR_QOT)
			{
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
				status = FOUND_DQOT;
			}
			else if (status == FOUND_DQOT)
			{
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_SQOT)
				continue ;
		}
		else if (arg[i] == '\'')
		{
			if (status == WAITING_FOR_QOT)
			{
				status = FOUND_SQOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_SQOT)
			{
				status = WAITING_FOR_QOT;
				ft_memmove(arg + i, arg + i + 1, ft_strlen(arg) - i);
				i--;
			}
			else if (status == FOUND_DQOT)
				continue ;
		}
	}
}
