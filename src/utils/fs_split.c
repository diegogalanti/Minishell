/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:34:21 by digallar          #+#    #+#             */
/*   Updated: 2024/03/26 08:32:56 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_char(char const *s, char c)
{
	int	i;
	int	just_found;
	int	count;

	count = 0;
	i = 0;
	just_found = 0;
	if (s[0] == 0)
		return (-1);
	while (s[i])
	{
		if (s[i] == c)
		{
			if (!just_found)
				count++;
			just_found = 1;
		}
		else
			just_found = 0;
		i++;
	}
	return (count);
}

static int	index_of(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

static char	*get_next(t_data *data, char const *s, char c, int *new_index)
{
	int		index;
	char	*next;

	index = index_of(s + *new_index, c);
	next = safe_malloc(data, (index + 1) * sizeof(*next));
	if (!next)
		return (0);
	next[index] = 0;
	ft_memcpy(next, s + *new_index, index * sizeof(*next));
	*new_index += index;
	while (s[*new_index] == c)
		*new_index = *new_index + 1;
	return (next);
}

char	**fs_split(t_data *data, char const *s, char c)
{
	int		count;
	char	**result;
	int		i;
	int		new_index;

	count = count_char(s, c);
	result = safe_malloc(data, (count + 2) * sizeof(*result));
	if (!result)
		return (0);
	result[count + 1] = 0;
	i = -1;
	new_index = 0;
	while (++i < count + 1)
	{
		result[i] = get_next(data, s, c, &new_index);
		if (result[i] == 0)
			return (0);
	}
	return (result);
}
