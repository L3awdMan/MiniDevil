/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:48:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/01/15 23:47:40 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//==================== FUNCTION: extract_quoted ===========================
//
// PURPOSE:
//    Extract a quoted section of text INCLUDING the quotes themselves
//    This is a KEY CHANGE from before - we now KEEP quotes in tokens
//
// PSEUDO-CODE:
// //    quote = str[0]              // Save ' or "
// //    end = 1                     // Start after opening quote
// //
// //    while str[end] exists AND str[end] != quote:
// //        end++                   // Find closing quote
// //
// //    if str[end] != quote:       // Unclosed quote?
// //        print error
// //        return NULL
// //
// //    content = substr(str, 0, end + 1)  // KEEP QUOTES: from 0 to end+1
// //    *len = end + 1                      // Move past closing quote
// //    return content'

char	*extract_quoted(char *str, int *len)
{
	char	quote;
	int		end;
	char	*content;

	quote = str[0];
	end = 1;
	while (str[end] && str[end] != quote)
		end++;
	if (str[end] != quote)
		return (ft_putstr_fd("minishell: unclosed quote\n", 2), NULL);
	content = ft_substr(str, 0, end + 1);
	*len = end + 1;
	return (content);
}

//==================== FUNCTION: extract_unquoted =========================
//
// PURPOSE:
//    Extract text that is NOT quoted - regular word characters
//
// PSEUDO-CODE:
//    i = 0
//    while str[i] exists AND NOT is_word_end(str[i]):
//        i++
//    *len = i
//    return substr(str, 0, i)

char	*extract_unquoted(char *str, int *len)
{
	int	i;

	i = 0;
	while (str[i] && !is_word_end(str[i]))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}
