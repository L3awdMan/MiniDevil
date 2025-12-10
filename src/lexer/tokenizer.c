/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 16:15:26 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "token.h"

int	process_quoted_token(char *input, t_token **head)
{
	char			*word;
	t_quote_type	quote_type;
	int				quote_len;
	t_token			*token;

	word = handle_quotes(input, &quote_type);
	if (!word)
		return (-1);
	quote_len = find_closing_quote(input, input[0], 1) + 2; // +2 fix
	token = create_token(TOKEN_WORD, word);
	if (!token)
		return (free(word), -1);
	token->quote_type = quote_type;
	add_token(head, token);
	free(word);
	return (quote_len);
}

int	process_operator_token(char *input, t_token **head)
{
	int				len;
	t_token_type	type;
	char			*op_str;

	type = get_operator_token_type(input, &len);
	op_str = ft_substr(input, 0, len);
	if (!op_str)
		return (-1);
	add_token(head, create_token(type, op_str));
	free(op_str);
	return (len);
}

int	process_word_token(char *input, t_token **head)
{
	char	*word;
	int		len;

	word = extract_word(input, &len);
	if (!word)
		return (-1);
	add_token(head, create_token(TOKEN_WORD, word));
	free(word);
	return (len);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	int		i;
	int		len;

	if (!input)
		return (NULL);
	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '"')
			len = process_quoted_token(input + i, &head);
		else if (is_operator(input[i]))
			len = process_operator_token(input + i, &head);
		else
			len = process_word_token(input + i, &head);
		if (len < 0) // handling return -1
			return (free_token_list(head), NULL);
		i += len;
	}
	return (head);
}
