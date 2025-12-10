/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 21:53:31 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "token.h"
#include "minishell.h"

static char	*join_and_free(char *s1, char *s2) // gonna add this to libft later...
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

//==================================================
//================ WORD EXTRACTION =================
//==================================================

/**
 * @brief extract a quoted part of the word (len is updated to include the quotes)
 * @return the content without quotes
 * @warning (this will be a problem for the expansion, I followed what you did but its better to keep quotes and remove them in expansion later)
 */
static char	*extract_quoted(char *str, int *len)
{
	char	quote;
	int		end;
	char	*content;

	quote = str[0];
	end = 1;
	while (str[end] && str[end] != quote) // find closing quote
		end++;
	if (str[end] != quote)
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		return (NULL);
	}
	content = ft_substr(str, 1, end - 1);
	*len = end + 1; // go the next character (right after the closing quote)
	return (content);
}

/**
 * @brief extract normal text until a separator (space, operator or quote)
 */
static char *extract_unquoted(char *str, int *len)
{
	int	i;

	i = 0;
	while (str[i] && !is_word_end(str[i]))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

//==================================================
//=============== TOKEN PROCESSING =================
//==================================================

/**
 * @brief Get the next chunk of the word (quoted or unquoted)
 */
static char	*get_next_chunk(char *input, int *chunk_len)
{
	if (input[0] == '\'' || input[0] == '"')
		return (extract_quoted(input, chunk_len));
	return (extract_unquoted(input, chunk_len));
}

/**
 * @brief Loops through the input to join the quoted and unquoted chunks into one token
 * Example: 'Ham'"id" -> Hamid (fist chunk is 'Ham' / second is "id")
 */
int	process_word_token(char *input, t_token **head)
{
	char	*result;
	char	*part;
	int		total_len;
	int		part_len;

	total_len = 0;
	result = ft_strdup("");
	if (!result)
		return (-1);
	while (input[total_len] && !is_whitespace(input[total_len])
		&& !is_operator(input[total_len]))
	{
		part = get_next_chunk(input + total_len, &part_len);
		if (!part)
			return (free(result), -1);
		result = join_and_free(result, part);
		if (!result)
			return (-1);
		total_len += part_len;
	}
	if (ft_strlen(result))
		add_token(head, create_token(TOKEN_WORD, result));
	free(result);
	return(total_len);
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

//==================================================
//================ TOKENIZER LOOP ==================
//==================================================

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
		if (is_operator(input[i]))
			len = process_operator_token(input + i, &head);
		else
			len = process_word_token(input + i, &head);
		if (len == -1)
			return (free_token_list(head), NULL);
		i += len;
	}
	return (head);
}


// int	process_quoted_token(char *input, t_token **head)
// {
// 	char			*word;
// 	t_quote_type	quote_type;
// 	int				quote_len;
// 	t_token			*token;

// 	word = handle_quotes(input, &quote_type);
// 	if (!word)
// 		return (-1);
// 	quote_len = find_closing_quote(input, input[0], 1) + 2; // +2 fix
// 	token = create_token(TOKEN_WORD, word);
// 	if (!token)
// 		return (free(word), -1);
// 	token->quote_type = quote_type;
// 	add_token(head, token);
// 	free(word);
// 	return (quote_len);
// }

// int	process_word_token(char *input, t_token **head)
// {
// 	char	*word;
// 	int		len;

// 	word = extract_word(input, &len);
// 	if (!word)
// 		return (-1);
// 	add_token(head, create_token(TOKEN_WORD, word));
// 	free(word);
// 	return (len);
// }

