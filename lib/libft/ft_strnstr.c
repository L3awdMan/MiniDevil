/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 23:03:22 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:37:21 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	int		i;
	size_t	len_to_find;

	len_to_find = ft_strlen(to_find);
	if (len_to_find == 0)
		return ((char *)str);
	if (len == 0)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == to_find[0])
			if (ft_strncmp((str + i), to_find, len_to_find) == 0)
				if ((i + len_to_find) <= len)
					return ((char *)str + i);
		i++;
	}
	return (NULL);
}
