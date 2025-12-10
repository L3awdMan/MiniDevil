/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:16:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 01:12:54 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "token.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

//======================== FUNCTION: get_operator_token_type ================
//
// PURPOSE:
//    Determine what type of operator token we have
//
// RETURN:
//    t_token_type - Type of operator token
//
// PARAMETERS:
//    char *str - String starting at operator position
//    int *len  - Output: how many chars the operator takes (1 or 2)
//
// VARIABLES:
//    None
//
// ALGORITHM:
//    1. Check if str[0] and str[1] form double operator:
//       - If str[0] == '>' AND str[1] == '>':
//         * Set *len = 2
//         * Return TOKEN_APPEND
//       - If str[0] == '<' AND str[1] == '<':
//         * Set *len = 2
//         * Return TOKEN_HEREDOC
//    2. Check single operators:
//       - If str[0] == '|': set *len = 1, return TOKEN_PIPE
//       - If str[0] == '>': set *len = 1, return TOKEN_REDIR_OUT
//       - If str[0] == '<': set *len = 1, return TOKEN_REDIR_IN
//    3. Default: set *len = 1, return TOKEN_WORD
//
// EXAMPLE:
//    ">>" â†’ len=2, TOKEN_APPEND
//    ">"  â†’ len=1, TOKEN_REDIR_OUT
//    "|"  â†’ len=1, TOKEN_PIPE

t_token_type	get_operator_token_type(char *str, int *len)
{
	*len = 1;
	if (str[0] == '>' && str[1] == '>')
	{
		*len = 2;
		return (TOKEN_APPEND);
	}
	else if (str[0] == '<' && str[1] == '<')
	{
		*len = 2;
		return (TOKEN_HEREDOC);
	}
	else if (str[0] == '|')
		return (TOKEN_PIPE);
	else if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	else if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	else
		return (TOKEN_WORD);
}

//======================== FUNCTION: extract_word ===========================
//
// PURPOSE:
//    Extract one word from input (stops at space or operator)
//
// RETURN:
//    char * - The extracted word
//
// PARAMETERS:
//    char *str - String to extract from
//    int *len  - Output: length of extracted word
//
// VARIABLES:
//    int i - Counter for word length
//
// ALGORITHM:
//    1. Initialize i
//    2. Loop while str[i] exists AND (not whitespace AND not operator)
//       - Increment i
//    3. Set *len to total characters counted
//    4. Extract word
//    5. Return extracted word
//
// EXAMPLE:
//    "hello world" â†’ len=5, return "hello"
//    "cat|grep"    â†’ len=3, return "cat"

char	*extract_word(char *str, int *len)
{
	int		i;
	char	*result;

	i = 0;
	while (str[i] && (!is_operator(str[i]) && !is_whitespace(str[i])))
		i++;
	*len = i;
	result = ft_substr(str, 0, i);
	return (result);
}
