/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 11:58:35 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 12:54:15 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_sort_argument(va_list *args, char c, size_t *len)
{
	char	b;

	if (c == 'c')
	{
		b = va_arg(*args, int);
		*len += write(1, &b, 1);
	}
	if (c == 's')
		ft_putstr_count(va_arg(*args, char *), len);
	if (c == 'd' || c == 'i')
		ft_putnbr_len(va_arg(*args, int), len);
	if (c == 'x')
		ft_puthexa_len(va_arg(*args, unsigned int), len, 'l');
	if (c == 'X')
		ft_puthexa_len(va_arg(*args, unsigned int), len, 'U');
	if (c == 'u')
		ft_unsigned_int(va_arg(*args, unsigned int), len);
	if (c == 'p')
		ft_print_address(va_arg(*args, void *), len);
	if (c == '%')
		*len += write(1, "%", 1);
	else
		return ;
	return ;
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	len;
	int		i;

	if (format == NULL || !format || *format == '\0')
		return (0);
	va_start(args, format);
	i = 0;
	len = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			ft_sort_argument(&args, format[i], &len);
		}
		else
		{
			len += write(1, &format[i], 1);
		}
		i++;
	}
	va_end(args);
	return (len);
}
