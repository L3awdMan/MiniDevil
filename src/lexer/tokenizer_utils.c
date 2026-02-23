/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:16:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 23:01:58 by baelgadi         ###   ########.fr       */
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
