/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 17:35:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 07:26:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"

/**
 * @brief Check if a token is a `$` connected to a quoted token
 * 
 * Detects the pattern:
 * $"TOKEN_WORD" or $'TOKEN_WORD' + connected + next has quotes
 * 
 * @param token Token to inspect
 * @return 1 if the pattern is valid and 0 if not
 */
int	is_dollar_quote(t_token *token)
{
	if (!token->connected || !token->next)
		return (0);
	if (ft_strncmp(token->value, "$", 2) != 0)
		return (0);
	return (token->next->quote_type != QUOTE_NONE);
}

/**
 * @brief Check if a character is valid in a variable name
 * 
 * @param c Character to check
 * @return 1 if alphanumeric or `_` and 0 otherwise
 */
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * @brief Extract a variable name from after the `$`
 * 
 * If the first char is a digit, it returns the only digit (matching BASH).
 * Otherwise it scans for valid variable characters (alphanumeric and `_`)
 * 
 * @param str String starting after `$`
 * @param len Number of characters consumed
 * @return Newly allocated variable name
 */
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

/**
 * @brief Append a single character to an allocated string
 * 
 * @param str Original string
 * @param c Character to append
 * @return New string with c appended or NULL on failure
 * @warning original str is freed
 */
char	*append_char(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	len = ft_strlen(str);
	new_str = ft_calloc(sizeof(char), len + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
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

/**
 * @brief Concatenates 2 strings and frees the first
 * 
 * If s2 is NULL it returns s1 unchanged
 * 
 * @param s1 First string
 * @param s2 Second string
 * @return Concatenated string or NULL on failure
 */
char	*append_str(char *s1, char *s2)
{
	char	*result;

	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
