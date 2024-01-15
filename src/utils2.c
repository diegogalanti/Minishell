/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:17 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/15 10:30:08 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ff_strdup(t_command *command, char *s)
{
	char	*cpy;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	cpy = (char *)command_safe_malloc(command, (sizeof(*cpy) * (len + 1)));
	if (!cpy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		cpy[i] = s[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	*fs_substr(char const *s, unsigned int start, size_t len, t_command *c)
{
	char	*substr;
	size_t	i;
	size_t	strlen;

	strlen = ft_strlen(s);
	if (!s)
		return (NULL);
	if ((size_t)start > strlen)
		return (ff_strdup(c, ""));
	if (len > (strlen - start))
		len = (strlen - start);
	substr = (char *)command_safe_malloc(c, sizeof(*substr) * (len + 1));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

int	ft_isspace(int c)
{
	c = (unsigned char)c;
	return (c == '\n' || c == '\t' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r');
}
