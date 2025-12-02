/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:07:19 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:37:21 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **result, int i)
{
	while (i > 0)
	{
		i--;
		free(result[i]);
	}
	free(result);
	return (0);
}

static int	check_seperator(char c, char sep)
{
	return (c == sep);
}

static int	count_strings(const char *str, char sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && check_seperator(str[i], sep))
			i++;
		if (str[i])
			count++;
		while (str[i] && !check_seperator(str[i], sep))
			i++;
	}
	return (count);
}

static char	*ft_word(const char *str, char sep)
{
	int		i;
	int		len_word;
	char	*word;

	len_word = 0;
	while (str[len_word] && !check_seperator(str[len_word], sep))
		len_word++;
	word = (char *)malloc(len_word + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(const char *s, char c)
{
	int		i;
	int		size_strs;
	char	**result;

	size_strs = count_strings(s, c);
	result = (char **)malloc((size_strs + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && check_seperator(*s, c))
			s++;
		if (*s)
		{
			result[i] = ft_word(s, c);
			if (!result[i])
				return (free_array(result, i));
			i++;
		}
		while (*s && !check_seperator(*s, c))
			s++;
	}
	result[i] = NULL;
	return (result);
}
