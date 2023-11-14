/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:31:49 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 12:58:56 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)

{
	unsigned char	*m;
	size_t			i;

	m = (void *)s;
	i = 0;
	while (i < n)
	{
		if (m[i] == (unsigned char)c)
			return (&m[i]);
		i++;
	}
	return (NULL);
}
