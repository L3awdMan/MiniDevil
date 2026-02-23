/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:24:49 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*extract_quoted(char *str, int *len, t_quote_type *qtype)
{
	char	quote;
	int		end;

	quote = str[0];
	end = 1;
	while (str[end] && str[end] != quote)
		end++;
	if (!str[end])
		return (ft_putstr_fd("minishell: unclosed quote\n", 2), NULL);
	if (quote == '\'')
		*qtype = QUOTE_SINGLE;
	else
		*qtype = QUOTE_DOUBLE;
	*len = end + 1;
	return (ft_substr(str, 1, end - 1));
}

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
		if (!token)
			return (free(chunk), -1);
		token->quote_type = qtype;
		i += len;
		token->connected = (s[i] && !is_whitespace(s[i]) && !is_operator(s[i]));
		add_token(head, token);
		free(chunk);
	}
	return (i);
}

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
			return (free_token_list(head), NULL);
		i += len;
	}
	return (head);
}
