/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:14:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:28:38 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

/**
 * @file token.h
 * @brief Lexer (tokenizer) and expander function prototypes
 *
 * The lexer breaks input into tokens
 * The expander handles $variable expansion
 */

# include "minishell.h"
# include "structs.h"
//==================================================
//=============== TOKEN MANAGEMENT =================
//==================================================

/**
 * @brief Create a new token
 *
 * @param type Token type (WORD, PIPE, ...)
 * @param value Text content of the token
 * @return New token or NULL on failure
 */
t_token			*create_token(t_token_type type, char *value);

/**
 * @brief Free a single token
 *
 * @param token The token we need to free
 */
void			free_token(t_token *token);

/**
 * @brief Free entire token list
 *
 * @param head First token in the list
 */
void			free_token_list(t_token *head);

/**
 * @brief Add token to end of list
 *
 * @param head Pointer to the list head
 * @param new_token Token to add
 */
void			add_token(t_token **head, t_token *new_token);

/**
 * @brief Count tokens in list
 *
 * @param head First token
 * @return Number of tokens
 */
int				token_list_size(t_token *head);

//==================================================
//================ QUOTE HANDLING ==================
//==================================================

/**
 * @brief Find matching closing quote
 *
 * @param str Input string
 * @param quote_char Quote to match (' or ")
 * @param start Position after opening quote
 * @return Position of closing quote or -1 if not found
 */
int				find_closing_quote(char *str, char quote_char, int start);

/**
 * @brief Extract the content between quotes
 *
 * @param str Input string
 * @param start Position after opening quote
 * @param end Position of closing quote
 * @return Extracted content (allocated!)
 */
char			*extract_quoted_content(char *str, int start, int end);

/**
 * @brief Process quoted string and track the quote type (problem for now)
 *
 * @param input Input string starting at quote
 * @param is_quoted <----- !!!!!!!!!! REPLACE
 * @return Processed string with the quotes removed
 */

char			*handle_quotes(char *input, t_quote_type *quote_type);
/**
 * @brief
 *
 * @param value
 * @return int
 */
int				was_quoted(const char *value);

//==================================================
//=============== TOKENIZER UTILS ==================
//==================================================

/**
 * @brief Check if a character is an operator
 *
 * @param c Character to check
 * @return 1 if operator and 0 if not
 */
int				is_operator(char c);

/**
 * @brief Check if a character is a whitespace
 *
 * @param c Character to check
 * @return 1 if whitespace and 0 if not
 */
int				is_whitespace(char c);

/**
 * @brief Get the token type for an operator and its length
 *
 * @param str String starting at operator
 * @param len Length of operator (1 or 2)
 * @return The type for this operator
 */
t_token_type	get_operator_token_type(char *str, int *len);

/**
 * @brief Extract a word
 *
 * @param str String to extract from
 * @param len Length of extracted word
 * @return Extracted word (allocated!)
 */
char			*extract_word(char *str, int *len);

//==================================================
//================ MAIN TOKENIZER ==================
//==================================================

/**
 * @brief Process a quoted token from the input
 *
 * @param input Input string at quote position
 * @param head The token list to add to
 * @return Number of characters consumed
 */
int				process_quoted_token(char *input, t_token **head);

/**
 * @brief Process an operator token fron the input
 *
 * @param input Input string at operator position
 * @param head The token list to add to
 * @return Number of characters consumed
 */
int				process_operator_token(char *input, t_token **head);

/**
 * @brief Process a word token fron the input
 *
 * @param input Input string at word position
 * @param head The token list to add to
 * @return Number of characters consumed
 */
int				process_word_token(char *input, t_token **head);

/**
 * @brief Main tokenizer
 *
 * Convert input to token list
 *
 * @param input Raw input string from the user
 * @return Linked list of tokens or NULL on syntax error
 */
t_token			*tokenize(char *input);

//==================================================
//================ EXPANDER UTILS ==================
//==================================================

/**
 * @brief Check if a character is valid in variable name
 *
 * @param c Character to check
 * @return 1 if alphanumeric or undescore, 0 otherwise
 */
int				is_var_char(char c);

/**
 * @brief Extract variable name from string
 *
 * @param str String starting after $
 * @param len Length of variable name
 * @return Variable name (allocated!)
 */
char			*extract_var_name(char *str, int *len);

/**
 * @brief Append single character to string
 *
 * @param str Original string (freed)
 * @param c Character to append
 * @return New string with character appended
 */
char			*append_char(char *str, char c);

/**
 * @brief Concatenate 2 strings
 *
 * @param s1 First string (freed)
 * @param s2 Second string (freed)
 * @return Combined string of both
 */
char			*append_str(char *s1, char *s2);

/**
 * @brief
 *
 * @param str
 * @param env_list
 * @param in_quotes
 * @return char*
 */

//==================================================
//================ MAIN EXPANDER ===================
//==================================================

/**
 * @brief Expand variables in a string
 *
 * Behavior depends on quote type:
 * 	- Single: no expansion
 * 	- Double: expansion
 * 	- None: expansion
 *
 * @param str String to expand
 * @param env_list Environment variables
 * @param in_quotes <----- !!!!!!!!!! REPLACE
 * @return Expanded string (allocated!)
 */
char			*expand_variables(char *str, t_env *env_list,
					t_quote_type quote_type);

#endif
