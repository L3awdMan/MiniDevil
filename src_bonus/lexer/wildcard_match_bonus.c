/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 07:52:54 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/10 06:58:03 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"

/**
 * @brief Recursively match a wildcard pattern against a string
 * 
 * Counting '*' as wildcard and MASK_STAR as a literal `*`
 * 
 * @param pattern Wildcard pattern
 * @param str String to match
 * @return 1 if match and 0 if not
 */
int	wildcard_match(char *pattern, char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == MASK_STAR)
	{
		if (*str == '*')
			return (wildcard_match(pattern + 1, str + 1));
		return (0);
	}
	if (*pattern == '*')
	{
		while (*(pattern + 1) == '*')
			pattern++;
		if (wildcard_match(pattern + 1, str))
			return (1);
		if (*str && wildcard_match(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*str && *pattern == *str)
		return (wildcard_match(pattern + 1, str + 1));
	return (0);
}

/**
 * @brief Print an ambiguous redirect error to STDERR
 * 
 * @param name Wildcard pattern that caused the ambig error
 */
void	print_ambig(char *name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
}

/**
 * @brief Sort the array of filenames in ASCII order
 * 
 * Bubble sort
 * 
 * @param arr Array of strings
 * @param count Number of elements
 */
void	sort_matches(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strncasecmp(arr[j], arr[j + 1], -1) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Replace `*` with MASK_STAR to mark literal stars in quoted tokens
 * 
 * This way, the * in quoted context won't be treated as wildcards during
 * pattern matching
 * 
 * @param val String to mask
 * @return Newly allocated copy or NULL on failure
 */
static char	*mask_stars(char *val)
{
	char	*copy;
	int		i;

	copy = ft_strdup(val);
	if (!copy)
		return (NULL);
	i = 0;
	while (copy[i])
	{
		if (copy[i] == '*')
			copy[i] = MASK_STAR;
		i++;
	}
	return (copy);
}

/**
 * @brief Build a wildcard pattern from a chain of connected tokens
 * 
 * - Quoted tokens have their `*` masked so they are treated as literal
 * - Unquoted tokens with `*` set the has_wildcard value flag
 * 
 * @param start First token of the chain
 * @param has_wildcard Set to 1 if any unquoted `*` (wildcard) is found
 * @return Combined pattern string (newly allocated) or NULL on failure
 */
char	*build_chain_pattern(t_token *start, int *has_wildcard)
{
	char	*pattern;
	char	*chunk;
	t_token	*current;

	pattern = ft_strdup("");
	*has_wildcard = 0;
	current = start;
	while (pattern && current)
	{
		if (current->quote_type != QUOTE_NONE)
			chunk = mask_stars(current->value);
		else
		{
			if (ft_strchr(current->value, '*'))
				*has_wildcard = 1;
			chunk = ft_strdup(current->value);
		}
		if (chunk)
			pattern = ft_strjoin_and_free(pattern, chunk);
		if (!current->connected || !current->next)
			break ;
		current = current->next;
	}
	return (pattern);
}
