/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:24:45 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/09/21 13:03:04 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)

{
	char	*ptr;

	ptr = (char *)big;
	if (!*little)
		return (ptr);
	if (!*big)
		return (NULL);
	while ((len >= ft_strlen(little)) && (*ptr != '\0'))
	{
		if (*ptr == *little)
		{
			if (ft_strncmp(ptr, little, ft_strlen(little)) == 0)
				return (ptr);
		}
		ptr++;
		len--;
	}
	return (NULL);
}
