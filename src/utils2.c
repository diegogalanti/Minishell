/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:17 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/26 10:27:54 by tstahlhu         ###   ########.fr       */
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

/*char    *fs_strjoin_env_var(t_data *data, char *old_var, char *new_var)
{
	char	*cpy;
	size_t	i;

	i = 0;
	while (new_var[i] != '=')
		i++;
	fs_strjoin(new_var + i + 1)
	cpy = (char *)safe_malloc(data, (sizeof(*cpy) * (ft_strlen(old_var) + ft_strlen(new_var + i))));
	if (!cpy)
		return (NULL);
	j = 0;
	while (old_var[j] != '\0')
	{
		cpy[j] = old_var[i];
		i++;
	}
	if (s[i] == '+')
		i++;
	while (s[i] != '\0')
	{
		cpy[i] = s[i];
		i++;
	}   
	cpy[i] = '\0';
	return (cpy);
}*/

int	has_equal_sign(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i] != '\0')
	{
		if (str[i] == '=')
			return (1);
		if (33 > str[i] || str[i] >= 127)
			return (0);
	}
	return (0);
}

