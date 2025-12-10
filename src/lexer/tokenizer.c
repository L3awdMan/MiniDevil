/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:33:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:44:28 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "token.h"

//=================== FUNCTION: process_quoted_token ====================
//
// PURPOSE:
//    Handle quoted section and create token
//
// RETURN:
//    int - Length of entire quoted section (including quotes)
//
// PARAMETERS:
//    char *input    - Input string starting at quote
//    t_token **head - Token list to add to
//
// VARIABLES:
//    char *word          - Extracted quoted content (without quotes)
//    t_quote_type quote_type - Type of quote detected
//    int quote_len       - Length INCLUDING quotes
//    t_token *token      - Created token
//
// ALGORITHM:
//    1. Call handle_quotes on input (returns content, sets quote_type)
//    2. Check if word is NULL (error): return -1
//    3. Calculate total length: find closing quote position
//       - Use find_closing_quote(input, input[0], 1)
//       - quote_len = closing_pos + 2 (both quotes)
//    4. Create TOKEN_WORD token
//    5. Set token->quote_type to the detected quote type
//    6. Add to list
//    7. Free word
//    8. Return quote_len
int	process_quoted_token(char *input, t_token **head)
{
	char			*word;
	t_quote_type	quote_type;
	int				quote_len;
	t_token			*token;

	word = handle_quotes(input, &quote_type);
	if (!word)
		return (-1);
	quote_len = find_closing_quote(input, input[0], 1) + 1;
	token = create_token(TOKEN_WORD, word);
	if (!token)
		return (free(word), -1);
	token->quote_type = quote_type;
	add_token(head, token);
	free(word);
	return (quote_len);
}

//=================== FUNCTION: process_operator_token ==================
//
// PURPOSE:
//    Identifies operator type and creates appropriate token
//
// RETURN:
//    int - Length of operator (1 for |,<,> or 2 for <<,>>)
//
// PARAMETERS:
//    char *input    - Input string starting at operator
//    t_token **head - Token list to add to
//
// VARIABLES:
//    int len            - Operator length (1 or 2 chars)
//    t_token_type type  - Type of operator token
//    char *op_str       - Operator string for token value
//
// ALGORITHM:
//    1. Get operator type and length:
//       - type = get_operator_token_type(input, &len)
//
//    2. Extract operator string:
//       - op_str = ft_substr(input, 0, len)
//       - Example: For ">>", extract 2 chars: ">>"
//       - Check if op_str is NULL: return -1 (malloc error)
//
//    3.Add to token list:
//       - add_token(head, created_token)
//
//    4. Free temporary operator string:
//       - free(op_str)
//
//    5. Return length so caller knows how much to skip
//
// EXAMPLE:
//    Input: ">> output.txt"
//    → type=TOKEN_APPEND, len=2, op_str=">>", returns 2
//
//    Input: "| cat"
//    → type=TOKEN_PIPE, len=1, op_str="|", returns 1

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

//=================== FUNCTION: process_word_token ======================
//
// PURPOSE:
//    Handle regular word (non-quoted, non-operator) and create token
//
// RETURN:
//    int - Length of word extracted
//
// PARAMETERS:
//    char *input    - Input string starting at word
//    t_token **head - Token list to add to
//
// VARIABLES:
//    char *word - Extracted word
//    int len    - Length of word
//
// ALGORITHM:
//    1. Call extract_word to get the word and length
//    2. Check if word is NULL (error)
//    3. Create TOKEN_WORD token
//    4. Add to list
//    5. Free word
//    6. Return length

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

//=================== FUNCTION: tokenize ================================
//
// PURPOSE:
//    Main tokenization function - converts raw input into token list
//    This is the lexer's entry point (like doc.txt lexer() function)
//    Orchestrates all helper functions to build complete token list
//
// RETURN:
//    t_token * - Head of token linked list, NULL on error or empty input
//
// PARAMETERS:
//    char *input - Raw input string from readline
//                  Example: "echo 'hello' | cat > out.txt"
//
// VARIABLES:
//    t_token *head - Head of token list being built
//    int i         - Current position in input string
//    int len       - Length returned by process_* functions
//
// ALGORITHM:
//    1. Validate input:
//       - if (!input): return NULL
//
//    2. Initialize:
//       - head = NULL (empty token list)
//       - i = 0 (start at beginning)
//
//    3. Main tokenization loop - WHILE input[i] exists:
//
//       a. Skip whitespace (doc.txt approach):
//          - WHILE input[i] exists AND is_whitespace(input[i]):
//            * i++
//          - If reached end (!input[i]): break
//
//       b. Check for quotes (' or "):
//          - IF input[i] == '\'' OR input[i] == '"':
//            * len = process_quoted_token(input + i, &head)
//
//       c. Check for operators (|, <, >, <<, >>):
//          - IF is_operator(input[i]):
//            * len = process_operator_token(input + i, &head)
//
//       d. Regular word (default case):
//          - len = process_word_token(input + i, &head)
//
//        f. skip past : entire quoted section OR operator OR past word
//          - i += len
//
//    4. Return head of token list
//
// EXAMPLE FLOW:
//    Input: "echo 'hello' | cat"
//
//    i=0: Skip no whitespace
//    i=0: Not quote, not operator → process_word_token("echo...")
//         → Creates [TOKEN_WORD:"echo"], returns len=4
//         → i=4
//
//    i=4: Skip 1 whitespace → i=5
//    i=5: Quote detected → process_quoted_token("'hello'...")
//         → Creates [TOKEN_WORD:"hello"], returns len=7
//         → i=12
//
//    i=12: Skip 1 whitespace → i=13
//    i=13: Operator detected → process_operator_token("|...")
//          → Creates [TOKEN_PIPE:"|"], returns len=1
//          → i=14
//
//    i=14: Skip 1 whitespace → i=15
//    i=15: Not quote, not operator → process_word_token("cat")
//          → Creates [TOKEN_WORD:"cat"], returns len=3
//          → i=18
//
//    i=18: End of string → break
//
//    Result: [echo] → [hello] → [|] → [cat] → NULL

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
		i += len;
	}
	return (head);
}
