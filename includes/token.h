/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:14:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/04 00:11:57 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

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
//   type  - Type of token (from enum above)
//   value - Actual text content (e.g., "echo", "|", "file.txt")
//   next  - Next token in list

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

#endif
