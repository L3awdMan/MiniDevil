/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:14:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:24:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "structs.h"

// ────────────── token.c ────────────────

t_token			*create_token(t_token_type type, char *value);

void			free_token(t_token *token);

void			free_token_list(t_token *head);

void			add_token(t_token **head, t_token *new_token);

// ────────────── tokenizer_utils.c ────────────────

int				is_operator(char c);

int				is_whitespace(char c);

int				is_word_end(char c);

t_token_type	get_operator_token_type(char *str, int *len);

// ────────────── tokenizer.c ────────────────

int				process_word_token(char *s, t_token **head);

int				process_operator_token(char *input, t_token **head);

t_token			*tokenize(char *input);

// ────────────── expander_utils.c ────────────────

int				is_dollar_quote(t_token *tok);

int				is_var_char(char c);

char			*extract_var_name(char *str, int *len);

char			*append_char(char *str, char c);

char			*append_str(char *s1, char *s2);

// ────────────── expander.c ────────────────

char			*expand_variables(char *str, t_env *env_list,
					t_quote_type quote_type, int exit_status);

int				expand_all_tokens(t_token *tokens, t_shell *shell);

// ────────────── tilde_expand.c ────────────────

char			*expand_full(char *str, t_env *env, t_quote_type qt,
					int exit_status);

#endif
