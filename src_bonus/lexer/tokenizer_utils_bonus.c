/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:16:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/05 07:26:45 by baelgadi         ###   ########.fr       */
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
	return (c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
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
	*len = 2;
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_APPEND);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	*len = 1;
	if (str[0] == '(')
		return (TOKEN_LPAREN);
	if (str[0] == ')')
		return (TOKEN_RPAREN);
	if (str[0] == '|')
		return (TOKEN_PIPE);
	if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	return (TOKEN_WORD);
}
