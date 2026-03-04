/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:43:17 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"

/**
 * @brief Extract an unquoted text part until any delimiter is reached
 * 
 * @param str Input string
 * @param len Characters consumed
 * @param qtype Is set to QUOTE_NONE
 * @return Newly allocated substring or NULL if failed
 */
static char	*extract_unquoted(char *str, int *len, t_quote_type *qtype)
{
	int	i;

	i = 0;
	while (str[i] && !is_whitespace(str[i]) && !is_operator(str[i])
		&& str[i] != '\'' && str[i] != '"')
		i++;
	*len = i;
	*qtype = QUOTE_NONE;
	return (ft_substr(str, 0, i));
}

/**
 * @brief Extract a quoted text part, stripping the quotes
 * 
 * Finds the matching closing quote and sets qtype accordingly
 * 
 * @param str Input starting at the opening quote
 * @param len Total characters consumed (including the 2 quotes)
 * @param qtype Is set to QUOTE_SINGLE or QUOTE_DOUBLE
 * @return Content between quotes or NULL if unclosed (+ error msg)
 */
static char	*extract_quoted(char *str, int *len, t_quote_type *qtype)
{
	char	quote;
	int		end;

	quote = str[0];
	end = 1;
	while (str[end] && str[end] != quote)
		end++;
	if (!str[end])
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		return (NULL);
	}
	if (quote == '\'')
		*qtype = QUOTE_SINGLE;
	else
		*qtype = QUOTE_DOUBLE;
	*len = end + 1;
	return (ft_substr(str, 1, end - 1));
}

/**
 * @brief Process a word token composed of quoted and unquoted chunks
 * 
 * Parses adjacent chunks (for example hello"world"'!' -> 3 connected tokens)
 * - Each chunk becomes a separate token with its own quote_type and
 * token->connected = 1 if another chunk follows right after
 * 
 * @param s Input string at word position
 * @param head Pointer to token lsit head
 * @return Characters consumed or -1 on error (unclosed quotes or alloc failure)
 */
int	process_word_token(char *s, t_token **head)
{
	int				i;
	int				len;
	char			*chunk;
	t_quote_type	qtype;
	t_token			*token;

	i = 0;
	while (s[i] && !is_whitespace(s[i]) && !is_operator(s[i]))
	{
		if (s[i] == '\'' || s[i] == '"')
			chunk = extract_quoted(s + i, &len, &qtype);
		else
			chunk = extract_unquoted(s + i, &len, &qtype);
		if (!chunk)
			return (-1);
		token = create_token(TOKEN_WORD, chunk);
		free(chunk);
		if (!token)
			return (-1);
		token->quote_type = qtype;
		i += len;
		token->connected = (s[i] && !is_whitespace(s[i]) && !is_operator(s[i]));
		add_token(head, token);
	}
	return (i);
}

/**
 * @brief Process an operator token
 * 
 * Determines the operator type and length, creates a token and appends it
 * 
 * @param input Input at the operator character
 * @param head Pointer to token list head
 * @return Number of chars consumed (1 or 2) or -1 on allocation failure
 */
int	process_operator_token(char *input, t_token **head)
{
	int				len;
	t_token_type	type;
	char			*op;

	type = get_operator_token_type(input, &len);
	op = ft_substr(input, 0, len);
	if (!op)
		return (-1);
	add_token(head, create_token(type, op));
	free(op);
	return (len);
}

/**
 * @brief Tokenize the input string into a linked list of tokens
 * 
 * The tokenizer entry point: skips whitespaces and dispatches each segment to
 * process_operator_token() or process_word_token()
 * 
 * @param input Raw input string
 * @return Head of token list or NULL on error
 * @note On error it frees all tokens before returning NULL
 * @warning The caller has to free via free_token_list()
 */
t_token	*tokenize(char *input)
{
	t_token	*head;
	int		i;
	int		len;

	head = NULL;
	i = 0;
	while (input && input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			len = process_operator_token(input + i, &head);
		else
			len = process_word_token(input + i, &head);
		if (len < 0)
		{
			free_token_list(head);
			return (NULL);
		}
		i += len;
	}
	return (head);
}
