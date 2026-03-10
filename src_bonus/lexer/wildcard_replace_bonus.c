/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_replace_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 09:31:34 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/09 07:52:58 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "parser.h"
#include "libft.h"

/**
 * @brief Check if a wildcard after a redirect give 2+ matches
 * 
 * @param prev Previous token
 * @param count Number of wildcard matches
 * @param cur Current token
 * @return 1 if ambiguous and 0 if not
 */
int	check_ambig(t_token *prev, int count, t_token *cur)
{
	if (count > 1 && prev && is_redirection(prev->type))
	{
		print_ambig(cur->value);
		return (1);
	}
	return (0);
}

/**
 * @brief Build a linked list of tokens from matched file names
 * 
 * @param match_arr Array of matched file names
 * @param count Number of matches
 * @param last Set to the last token in the list currently being built
 * @return First token of the list or NULL on fail
 */
t_token	*build_match_list(char **match_arr, int count, t_token **last)
{
	t_token	*first;
	t_token	*new_token;
	int		i;

	first = NULL;
	*last = NULL;
	i = 0;
	while (i < count)
	{
		new_token = create_token(TOKEN_WORD, match_arr[i]);
		if (!new_token)
			break ;
		new_token->quote_type = QUOTE_NONE;
		if (!first)
			first = new_token;
		if (*last)
			(*last)->next = new_token;
		*last = new_token;
		i++;
	}
	return (first);
}

/**
 * @brief Free a chain of tokens from start to end
 * 
 * @param start First token to free
 * @param end Last token to free
 */
void	free_chain(t_token *start, t_token *end)
{
	t_token	*tmp;

	while (start && start != end)
	{
		tmp = start->next;
		free(start->value);
		free(start);
		start = tmp;
	}
	if (end)
	{
		free(end->value);
		free(end);
	}
}

/**
 * @brief Replace a token chain with wildcard matched tokens
 * 
 * BUilds new tokens from the amtch array and splice them into the list instead
 * of the old chain, then frees the replaced tokens
 * 
 * @param matches Array of matched filenames
 * @param count Number of matches
 * @param context Wildcard context that holds the list pointers
 */
void	replace_chain(char **matches, int count, t_wild_ctx *context)
{
	t_token	*first;
	t_token	*last;

	first = build_match_list(matches, count, &last);
	ft_free_strarray(matches);
	if (!first || !last)
	{
		*context->prev = context->end;
		return ;
	}
	last->next = context->end->next;
	if (*context->prev)
		(*context->prev)->next = first;
	else
		*context->head = first;
	free_chain(context->cur, context->end);
	*context->prev = last;
}
