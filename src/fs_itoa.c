/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fs_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 16:25:03 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/28 16:33:59 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_minint(char **str, t_data *data)
{
	*str = (char *)safe_malloc(data, sizeof(char) * 12);
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

char	*fs_itoa(int n, t_data *data)
{
	char	*str;
	int		size;

	if (n == -2147483648)
		return (ft_minint(&str, data));
	size = ft_size(n);
	str = (char *)safe_malloc(data, sizeof(char) * (size + 1));
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