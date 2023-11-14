/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:12:04 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:07:42 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nb_str(char const *s, char c)
{
	int	nb;

	nb = 0;
	while (*s != '\0')
	{
		while (*s == c && *s != '\0')
			s++;
		if (*s != '\0')
			nb++;
		while (*s != c && *s != '\0')
			s++;
	}
	return (nb);
}

static size_t	ft_len_str(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*s != c && *s != '\0')
	{
		s++;
		i++;
	}
	return (i);
}

static char	**ft_malloc_fail(char **arr)
{
	int	x;

	x = 0;
	while (arr[x])
	{
		free(arr[x]);
		arr[x] = NULL;
		x++;
	}
	free(arr);
	arr = NULL;
	return (0);
}

static char	**ft_createstr(char const *s, char c, char **arr, int nb)
{
	int	x;
	int	y;

	x = 0;
	while (x < nb)
	{
		y = 0;
		while (*s == c && *s != '\0')
			s++;
		arr[x] = (char *)malloc(sizeof(char) * (ft_len_str(s, c) + 1));
		if (!arr[x])
			return (ft_malloc_fail(arr));
		while (*s != c && *s != '\0')
		{
			arr[x][y++] = *(s++);
		}
		arr[x][y] = '\0';
		x++;
	}
	arr[x] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		nb;

	if (!s)
		return (0);
	nb = ft_nb_str(s, c);
	arr = (char **)malloc(sizeof(s) * (nb + 1));
	if (!arr)
		return (0);
	ft_createstr(s, c, arr, nb);
	return (arr);
}
