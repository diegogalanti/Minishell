/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:46:26 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 12:56:18 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putstr_count(char *str, size_t *len)
{
	int	i;

	if (str == NULL)
		str = "(null)";
	if (!str)
	{
		*len = -1;
		return ;
	}
	i = 0;
	while (str[i] != '\0')
	{
		*len += write(1, &str[i], 1);
		i++;
	}
}

void	ft_puthexa_len(unsigned long int n, size_t *len, char x)
{
	char	*base;

	if (n >= 16)
		ft_puthexa_len(n / 16, len, x);
	if (x == 'l' || x == 'a')
		base = "0123456789abcdef";
	if (x == 'U')
		base = "0123456789ABCDEF";
	*len += write(1, &base[n % 16], 1);
	return ;
}

void	ft_putnbr_len(int n, size_t *len)
{
	char	c;

	if (n == -2147483648)
	{
		*len += write(1, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		*len += write(1, "-", 1);
		n = n * (-1);
	}
	if (n > 9)
		ft_putnbr_len((n / 10), len);
	c = n % 10 + 48;
	*len += write(1, &c, 1);
	return ;
}

void	ft_unsigned_int(unsigned int n, size_t *len)
{
	char	c;

	if (n > 9)
		ft_unsigned_int(n / 10, len);
	c = n % 10 + 48;
	*len += write(1, &c, 1);
	return ;
}

void	ft_print_address(void *a, size_t *len)
{
	if (!a)
	{
		*len += write(1, "(nil)", 5);
		return ;
	}
	*len += write(1, "0x", 2);
	ft_puthexa_len((unsigned long int)a, len, 'a');
	return ;
}
