/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:17 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/14 09:55:44 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fs_strdup(t_data *data, char *s)
{
	char	*cpy;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	cpy = (char *)safe_malloc(data, (sizeof(*cpy) * (len + 1)));
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

/* string variable compare: compares environment variables
	This function is like ft_strcmp but it only compares the strings until an equal sign
	is found in s1.*/

int	ft_strvcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	if (s1 == NULL || s2 == NULL)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && s1[i] != '=')
		i++;
	if (s1[i] == '=' && (s2[i] == '\0' || s2[i] == '='))
		return (0);
	else
		return (1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ff_strjoin(char const *s1, char const *s2, t_data *data)
{
	char	*str;
	int		len;
	int		i;
	int		j;

	len = (ft_strlen(s1) + ft_strlen(s2));
	str = (char *)safe_malloc(data, sizeof(*str) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*fs_strjoin(char const *s1, char const *s2, t_command *command)
{
	char	*str;
	int		len;
	int		i;
	int		j;

	len = (ft_strlen(s1) + ft_strlen(s2));
	str = (char *)command_safe_malloc(command, sizeof(*str) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*ff_strdup(t_command *command, char *s)
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
