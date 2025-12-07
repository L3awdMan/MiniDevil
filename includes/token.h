/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:14:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/07 16:35:48 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "env.h"
# include "minishell.h"

//======================== TOKEN TYPES ENUM =================================
//
// PURPOSE: Define all possible token types in our shell
//
// TYPES:
//   TOKEN_WORD      - Regular word (command, argument, filename)
//   TOKEN_PIPE      - Pipe operator |
//   TOKEN_REDIR_IN  - Input redirection
//   TOKEN_REDIR_OUT - Output redirection >
//   TOKEN_APPEND    - Append redirection >>
//   TOKEN_HEREDOC   - Here document

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}					t_token_type;

//======================== TOKEN STRUCTURE ==================================
//
// PURPOSE: Represent one token from input
//
// FIELDS:
//   type   - Type of token (from enum above)
//   value  - Actual text content (e.g., "echo", "|", "file.txt")
//   quoted - Flag: 1 if token was inside quotes, 0 if not
//   next   - Next token in list
//
// WHY 'quoted' FLAG?
//    - Tells us if this token was originally in quotes
//    - Important for: preserving spaces, handling $expansion
//    - Example: 'hello world' â†’ value="hello world", quoted=1
//    - Example: hello â†’ value="hello", quoted=0
//
// EXAMPLE:
//    Input: echo 'hello world'
//    Token 1: type=TOKEN_WORD, value="echo", quoted=0, next=â†’Token2
//    Token 2: type=TOKEN_WORD, value="hello world", quoted=1, next=NULL

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

// Token creation/deletion & token list management
t_token				*create_token(t_token_type type, char *value);
void				free_token(t_token *token);
void				free_token_list(t_token *head);
void				add_token(t_token **head, t_token *new_token);
int					token_list_size(t_token *head);

// Quotes handling
int					find_closing_quote(char *str, char quote_char, int start);
char				*extract_quoted_content(char *str, int start, int end);
char				*handle_quotes(char *input, int *is_quoted);
int					was_quoted(const char *value);

// Tokenizer utils
int					is_operator(char c);
int					is_whitespace(char c);
t_token_type		get_operator_token_type(char *str, int *len);
char				*extract_word(char *str, int *len);

// Token processing helpers & main tokenizer
int					process_quoted_token(char *input, t_token **head);
int					process_operator_token(char *input, t_token **head);
int					process_word_token(char *input, t_token **head);
t_token				*tokenize(char *input);

// Expansion utilities (expander_utils.c)
int					is_var_char(char c);
char				*extract_var_name(char *str, int *len);
char				*append_char(char *str, char c);
char				*append_str(char *s1, char *s2);

// Main expansion function (expander.c)
char				*expand_variables(char *str, t_env *env_list,
						int in_quotes);

#endif
