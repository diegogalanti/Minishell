/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:38:20 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:05:29 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_get_start(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((s1[i] != set[j]) && (set[j] != '\0'))
		j++;
	while ((s1[i] == set[j]) && (s1[i] != '\0'))
	{
		i++;
		j = 0;
		while ((s1[i] != set[j]) && (set[j] != '\0'))
		{
			j++;
		}
	}
	return (i);
}

int	ft_get_end(char const *s1, char const *set, int start)
{
	int	i;
	int	j;

	i = ft_strlen(s1) - 1;
	j = 0;
	while ((s1[i] != set[j]) && (set[j] != '\0'))
		j++;
	while ((s1[i] == set[j]) && (start < i))
	{
		i--;
		j = 0;
		while ((s1[i] != set [j]) && (set[j] != '\0'))
			j++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		start;
	int		end;
	int		i;

	start = ft_get_start(s1, set);
	end = ft_get_end(s1, set, start);
	str = (char *)malloc(sizeof(*str) * (end - start + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (start + i <= end)
	{
		str[i] = s1[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
