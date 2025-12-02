/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:35:37 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:37:21 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	concatenation "behind the scene" while returning the total value
*/
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	original_len_dest;
	size_t	len_dest;
	size_t	len_src;
	size_t	i;

	original_len_dest = ft_strlen(dest);
	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	i = 0;
	if (!dest && n == 0)
		return (len_src);
	if (n <= len_dest)
		return (len_src + n);
	while (src[i] && (len_dest + 1 < n))
	{
		dest[len_dest] = src[i];
		i++;
		len_dest++;
	}
	dest[len_dest] = '\0';
	return (original_len_dest + len_src);
}
