/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:06:58 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:35:40 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   return the memory adress where the 'c' is located  */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	the_c;
	size_t			i;

	str = (unsigned char *)s;
	the_c = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == the_c)
			return ((void *)(s + i));
		i++;
	}
	return (NULL);
}
