/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:03:17 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/19 12:03:19 by tstahlhu         ###   ########.fr       */
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
		return(1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


