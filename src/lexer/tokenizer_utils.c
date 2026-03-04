/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:16:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:43:07 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/**
 * @brief Check if a character is a shell operator
 * 
 * @param c Character to test
 * @return 1 if operator and 0 otherwise
 */
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * @brief Check if a character is whitespace
 * 
 * @param c Character to test
 * @return 1 if whitespace and 0 otherwise
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/**
 * @brief Check if a character ends a word token
 * 
 * @param c Character to test
 * @return 1 if word ending and 0 otherwise
 */
int	is_word_end(char c)
{
	if (c == '\0')
		return (1);
	if (is_whitespace(c))
		return (1);
	if (is_operator(c))
		return (1);
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

/**
 * @brief Determine operator token type and its length in characters
 * 
 * @param str Input at the operator position
 * @param len Operator length (1 or 2)
 * @return Type of token for the operator
 */
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
