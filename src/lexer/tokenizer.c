/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/04 23:28:43 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

//=================== FUNCTION: process_quoted_token ====================
//
// PURPOSE:
//    Handle quoted section and create token
//
// RETURN:
//    int - Length of entire quoted section (including quotes)
//
// PARAMETERS:
//    char *input    - Input string starting at quote
//    t_token **head - Token list to add to
//
// VARIABLES:
//    char *word     - Extracted quoted content (without quotes)
//    int is_quoted  - Flag from handle_quotes
//    int quote_len  - Length INCLUDING quotes
//
// ALGORITHM:
//    1. Call handle_quotes on input (returns content, sets is_quoted)
//    2. Calculate total length: find closing quote position
//       - Use find_closing_quote(input, input[0], 1)
//       - quote_len = closing_pos + 1
//    3. Create TOKEN_WORD with word and is_quoted flag
//    4. Add to list
//    5. Free word
//    6. Return quote_len

int	process_quoted_token(char *input, t_token **head)
{
	char	*word;
	int		is_quoted;
	int		quote_len;

	world = handle_quotes(input, &is_quoted);
	quote_len = find_closing_quote(input, input[0], 1) + 1;
	add_token(head, create_token(TOKEN_WORD, word))
}
