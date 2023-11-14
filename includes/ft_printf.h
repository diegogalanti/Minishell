/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 16:52:36 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:01:14 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>/*for variadic arguments*/
# include <unistd.h>
# include <stdio.h>/*just for testing with prinf*/
# include <stdlib.h>/*for free*/

int		ft_printf(const char *format, ...);
void	ft_sort_argument(va_list *args, char c, size_t *len);
void	ft_putchar_count(char c, size_t *len);
void	ft_putstr_count(char *str, size_t *len);
void	ft_putnbr_len(int n, size_t *len);
void	ft_puthexa_len(unsigned long int n, size_t *len, char x);
void	ft_unsigned_int(unsigned int n, size_t *len);
void	ft_print_address(void *a, size_t *len);
char	*ft_itoa(int n);

#endif/*FT_PRINTF_H*/
