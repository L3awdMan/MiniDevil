/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 17:35:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/08 19:53:14 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "token.h"

int	is_dollar_quote(t_token *tok)
{
	if (!tok->connected || !tok->next)
		return (0);
	if (ft_strncmp(tok->value, "$", 2) != 0)
		return (0);
	return (tok->next->quote_type != QUOTE_NONE);
}

//======================== FUNCTION: is_var_char ============================
//
// PURPOSE:
//    Check if character is valid in variable name
//    Valid: a-z, A-Z, 0-9, _ (underscore)
//
// RETURN:
//    int - 1 if valid variable character, 0 if not
//
// PARAMETERS:
//    char c - Character to check
//
// VARIABLES:
//    None
//
// ALGORITHM:
//    1. Check if c is letter (a-z or A-Z)
//    2. Check if c is digit (0-9)
//    3. Check if c is underscore (_)
//    4. Return 1 if any true, 0 otherwise

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

//===================== FUNCTION: extract_var_name =======================
//
// PURPOSE:
//    Extract variable name after $ character
//    Variable names: letters, underscore, then letters/digits/underscore
//    Special case: $1, $2, etc. (positional params - single digit only)
//
// RETURN:
//    char * - Variable name (e.g., "USER", "HOME", "1", "2")
//
// PARAMETERS:
//    char *str - String starting AFTER the $ character
//    int *len  - Output: length of variable name extracted
//
// VARIABLES:
//    int i - Counter for variable name length
//
// ALGORITHM:
//    1. If first char is digit: extract only 1 char (positional param)
//    2. Else: extract while valid var char (letter/digit/underscore)
//    3. Set *len and return substring

char	*extract_var_name(char *str, int *len)
{
	int	i;

	i = 0;
	if (str[0] && ft_isdigit(str[0]))
	{
		*len = 1;
		return (ft_substr(str, 0, 1));
	}
	while (str[i] && is_var_char(str[i]))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

//===================== FUNCTION: append_char ============================
//
// PURPOSE:
//    Append a single character to a string (helper for building result)
//
// RETURN:
//    char * - New string with character appended, NULL on error
//
// PARAMETERS:
//    char *str - Original string (will be freed)
//    char c    - Character to append
//
// VARIABLES:
//    char *new_str - New string with character appended
//    int len       - Length of original string
//    int i
//
// ALGORITHM:
//    1. Get length of str: len = ft_strlen(str)
//    2. Allocate new string: malloc(len + 2)
//       - +1 for new char, +1 for null terminator
//    3. Copy original string to new_str
//    4. Add character at position len
//    5. Add null terminator at len + 1
//    6. Free original str
//    7. Return new_str
//
// EXAMPLE:
//    str = "Hello"
//    append_char(str, '!') -> "Hello!"

char	*append_char(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	len = ft_strlen(str);
	new_str = ft_calloc(sizeof(char), len + 2);
	if (!new_str)
		return (free(str), NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

//===================== FUNCTION: append_str =============================
//
// PURPOSE:
//    Append one string to another (helper for expansion)
//
// RETURN:
//    char * - Combined string, NULL on error
//
// PARAMETERS:
//    char *s1 - First string (will be freed)
//    char *s2 - Second string to append (NOT freed)
//
// VARIABLES:
//    char *result - Combined string
//
// ALGORITHM:
//    1. Use ft_strjoin to combine s1 and s2
//    2. Free s1 (we don't need it anymore)
//    3. Return result
//
// EXAMPLE:
//    s1 = "Hello "
//    s2 = "World"
//    append_str(s1, s2) -> "Hello World"
//    (s1 is freed, s2 is NOT freed)

char	*append_str(char *s1, char *s2)
{
	char	*result;

	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
