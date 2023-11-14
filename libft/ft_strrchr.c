/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:30:19 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:00:17 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)

{
	size_t	n;

	n = (ft_strlen(s) + 1);
	while (n--)
	{
		if (s[n] == (char)c)
			return ((char *)(s + n));
	}
	return (NULL);
}
