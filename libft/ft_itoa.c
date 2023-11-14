/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 12:40:09 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:02:12 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_minint(char **str)
{
	*str = (char *)malloc(sizeof(char) * 12);
	if (!*str)
		return (0);
	ft_strlcpy(*str, "-2147483648", 12);
	return (*str);
}

static int	ft_size(int n)
{
	int	size;

	size = 1;
	if (n < 0)
	{
		n = n * (-1);
		size++;
	}
	while (n > 9)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		size;

	if (n == -2147483648)
		return (ft_minint(&str));
	size = ft_size(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str[size--] = '\0';
	if (n == 0)
		str[size] = 48;
	if (n < 0)
	{
		str[0] = '-';
		n = n * (-1);
	}
	while (n > 0)
	{
		str[size--] = n % 10 + 48;
		n = n / 10;
	}
	return (str);
}
