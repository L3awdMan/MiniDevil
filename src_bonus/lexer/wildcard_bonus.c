/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:30:28 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/09 07:38:42 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "token.h"
#include "libft.h"

/**
 * @brief Read directory entries matching a wildcard pattern
 * 
 * This opens the current directory and collects the filenames that match the
 * pattern. Hidden files (that start with `.`) are skipped unless the pattern
 * also starts with `.`
 * 
 * @param pattern Wildcard pattern
 * @param count Number of matches found
 * @return Array of matching filenames or NULL if allocation failed
 */
static char	**read_matches(char *pattern, int *count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				cap;

	*count = 0;
	cap = 1024;
	matches = ft_calloc(sizeof(char *), cap + 1);
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (matches);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || pattern[0] == '.')
		{
			if (wildcard_match(pattern, entry->d_name) && *count < cap)
				matches[(*count)++] = ft_strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

/**
 * @brief Replace a chain of connected tokens with the wildcard matches
 * 
 * Builds the combined pattern from a chain, reads matching filenames from
 * the current directory and sorts them then splices them into the token list
 * - Paths containing `/` are skipped (no directory wildcards)
 * 
 * @param context Wildcard context (contains current chain and list pointers)
 */
static void	splice_into_list(t_wild_ctx *context)
{
	char	*pattern;
	char	**matches;
	int		count;

	pattern = build_chain_pattern(context->cur, &count);
	if (!pattern || !count || ft_strchr(pattern, '/'))
	{
		free(pattern);
		*context->prev = context->end;
		return ;
	}
	matches = read_matches(pattern, &count);
	free(pattern);
	if (!matches || !count || check_ambig(*context->prev, count, context->cur))
	{
		ft_free_strarray(matches);
		*context->prev = context->end;
		return ;
	}
	sort_matches(matches, count);
	replace_chain(matches, count, context);
}

/**
 * @brief Walk the token list & expand every wildcard chain
 * 
 * For every chain of connected word tokens, we check for unquoted `*` and
 * replace the chain with the sorted list of matching filenames
 * 
 * @param head Pointer to the token list head
 */
void	expand_wildcards(t_token **head)
{
	t_wild_ctx	context;
	t_token		*prev;
	t_token		*cur;
	t_token		*end;

	prev = NULL;
	cur = *head;
	context.prev = &prev;
	context.head = head;
	while (cur)
	{
		end = cur;
		while (end->connected && end->next)
			end = end->next;
		context.cur = cur;
		context.end = end;
		if (cur->type == TOKEN_WORD)
			splice_into_list(&context);
		else
			prev = end;
		cur = prev->next;
	}
}
