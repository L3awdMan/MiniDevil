/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:06:14 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:11:28 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include "structs.h"
#include "token.h"

//======================== FUNCTION: find_closing_quote =====================
//
// PURPOSE:
//    Find the closing quote that matches the opening quote
//
// RETURN:
//    int - Index of closing quote, or -1 if not found
//
// PARAMETERS:
//    char *str        - String to search in
//    char quote_char  - Quote character to find (' or ")
//    int start        - Where to start searching (after opening quote)
//
// VARIABLES:
//    int i - Index counter
//
// ALGORITHM:
//    1. Start loop at 'start' position: i = start
//    2. While str[i] exists:
//       - If str[i] == quote_char: return i (found closing quote!)
//       - Move forward: i++
//    3. If loop ends: return -1 (no closing quote found)

int	find_closing_quote(char *str, char quote_char, int start)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (str[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

//======================== FUNCTION: extract_quoted_content =================
//
// PURPOSE:
//    Extract content between quotes (without the quotes themselves)
//
// RETURN:
//    char * - Content between quotes, or NULL on error
//
// PARAMETERS:
//    char *str   - Full string
//    int start   - Index of opening quote
//    int end     - Index of closing quote
//
// VARIABLES:
//    int len    - Length of content (end - start - 1)
//    char *result - Extracted content
//
// ALGORITHM:
//    1. Calculate length
//    2. Use ft_substr to extract
//    3. Return result
char	*extract_quoted_content(char *str, int start, int end)
{
	int		len;
	char	*result;

	len = end - start - 1;
	result = ft_substr(str, start + 1, len);
	return (result);
}

//======================== FUNCTION: handle_quotes ==========================
//
// PURPOSE:
//    Process a string that may contain quotes, merge quoted content
//
// RETURN:
//    char * - Processed string with quotes removed
//
// PARAMETERS:
//    char *input   - Input string (may have quotes)
//    int *is_quoted - Output: 1 if string was quoted, 0 if not
//
// VARIABLES:
//    char quote_char - Which quote we found (' or ")
//    int close_pos   - Position of closing quote
//    char *result    - Extracted string without quotes (return value)
//
// ALGORITHM:
//    1. Check first character:
//       - If it's `'` or `"` : we have quoted content
//       - Store quote_char
//       - Find closing quote
//       - If close_pos == -1 print : "UNCLOSED QUOTE ERROR"
//       - Extract content between quotes
//       - Set *is_quoted = 1
//       - Return extracted content
//    2. No quotes found - return duplicate of input
char	*handle_quotes(char *input, t_quote_type *quote_type)
{
	char	quote_char;
	int		close_pos;
	char	*result;

	*quote_type = QUOTE_NONE;
	if (input[0] == '\'' || input[0] == '"')
	{
		quote_char = input[0];
		close_pos = find_closing_quote(input, quote_char, 1);
		if (close_pos == -1)
		{
			ft_printf("ERROR : UNCLOSED QUOTE \n");
			*quote_type = QUOTE_NONE;
			return (NULL);
		}
		result = extract_quoted_content(input, 0, close_pos);
		if (quote_char == '\'')
			*quote_type = QUOTE_SINGLE;
		else if (quote_char == '"')
			*quote_type = QUOTE_DOUBLE;
		return (result);
	}
	return (ft_strdup(input));
}

//======================== FUNCTION: was_quoted =============================
//
// PURPOSE:
//    Detect if a token value was likely originally quoted
//
// RETURN:
//    int - 1 if value was likely quoted, 0 if not
//
// PARAMETERS:
//    const char *value - Token value to check
//
// VARIABLES:
//    None
//
// ALGORITHM:
//    1. Check if value is NULL
//    2. Check if value contains spaces (strong indicator of quoting)
//    3. Check if value contains shell special characters
//    4. Return 1 if any condition met, 0 otherwise
//
// LOGIC:
//    - If a value contains spaces, it must have been quoted
//    - If a value contains special chars like |, >, <, they're usually quoted
//    - This is a heuristic - not 100% accurate but good enough for most cases

int	was_quoted(const char *value)
{
	if (!value)
		return (0);
	if (ft_strchr(value, ' ') || ft_strchr(value, '\t'))
		return (1);
	if (ft_strchr(value, '|') || ft_strchr(value, '>') || ft_strchr(value, '<'))
		return (1);
	if (ft_strchr(value, '$') || ft_strchr(value, '*') || ft_strchr(value, '?'))
		return (1);
	return (0);
}
