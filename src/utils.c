/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:17 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/26 12:35:56 by tstahlhu         ###   ########.fr       */
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


char	*fs_strdup_env_var(t_data *data, char *s)
{
	char	*cpy;
	int		i;
	int		j;

	cpy = (char *)safe_malloc(data, (sizeof(*cpy) * (ft_strlen(s) + 1)));
	if (!cpy)
		return (NULL);
	i = 0;
	while (s[i] != '=' && s[i] != '+' && s[i] != '\0')
	{
		cpy[i] = s[i];
		i++;
	}
	j = i;
	if (s[i] == '+')
		i++;
	while (s[i] != '\0')
		cpy[j++] = s[i++];
	cpy[j] = '\0';
	return (cpy);
}

/* string variable compare: compares environment variables
	This function is like ft_strcmp 
	but it only compares the strings until an equal sign is found in s1.*/

int	ft_strvcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	if (s1 == NULL || s2 == NULL)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && s1[i] != '=')
		i++;
//	if (s1[i] == '=' && (s2[i] == '\0' || s2[i] == '=' || s2[i] == '+'))
	//if ((s1[i] == '=' || s1[i] == '\0') && s2[i] == '\0')
	if (s1[i] == s2[i])
		return (0);
	else if (s1[i] == '\0')
	{
		if (s2[i] == '=' || (s2[i] == '+' && s2[i + 1] == '='))
			return (0);
	}
	else if (s1[i] == '=' && ((s2[i] == '+' && s2[i + 1] == '=') || s2[i] == '\0'))
		return (0);
	else
		return (1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

