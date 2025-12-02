/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:15:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:37:21 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   cut unwanted characters "set" in the bigining and the end   */

#include "libft.h"

static int	ft_check_set(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	len_s1;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	while (s1)
	{
		if (ft_check_set(((char)*s1), set) == 1)
			s1++;
		else
			break ;
	}
	len_s1 = ft_strlen(s1);
	while (len_s1 != 0)
	{
		if (ft_check_set(s1[len_s1 - 1], set) == 1)
			len_s1--;
		else
			break ;
	}
	str = (char *)malloc(len_s1 + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, (char *)s1, (len_s1 + 1));
	return (str);
}
