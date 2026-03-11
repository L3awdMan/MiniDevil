/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:14:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/11 03:00:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "structs.h"

# ifdef BONUS
#  define MASK_DOLLAR '\x02'
#  define MASK_STAR '\x01'
# endif

/* ────────────── token.c ──────────────── */

t_token			*create_token(t_token_type type, char *value);

void			free_token(t_token *token);

void			free_token_list(t_token *head);

void			add_token(t_token **head, t_token *new_token);

/* ────────────── tokenizer_utils.c ──────────────── */

int				is_operator(char c);

int				is_whitespace(char c);

int				is_word_end(char c);

t_token_type	get_operator_token_type(char *str, int *len);

/* ────────────── tokenizer.c ──────────────── */

int				process_word_token(char *s, t_token **head);

int				process_operator_token(char *input, t_token **head);

t_token			*tokenize(char *input);

/* ────────────── expander_utils.c ──────────────── */

int				is_dollar_quote(t_token *tok);

int				is_var_char(char c);

char			*extract_var_name(char *str, int *len);

char			*append_char(char *str, char c);

char			*append_str(char *s1, char *s2);

/* ────────────── expander.c ──────────────── */

char			*expand_variables(char *str, t_env *env_list,
					t_quote_type quote_type, int exit_status);

int				expand_all_tokens(t_token *tokens, t_shell *shell);

/* ────────────── tilde_expand.c ──────────────── */

char			*expand_full(char *str, t_env *env, t_quote_type qt,
					int exit_status);

/* ────────────── B O N U S ──────────────── */

/** @cond BONUS */

int				defer_expand_tokens(t_token *tokens, t_shell *shell);

void			expand_wildcards(t_token **head);

int				wildcard_match(char *pattern, char *str);

void			print_ambig(char *name);

void			sort_matches(char **arr, int count);

char			*build_chain_pattern(t_token *start, int *has_wildcard);

void			replace_chain(char **m, int cnt, t_wild_ctx *ctx);

t_token			*build_match_list(char **m, int cnt, t_token **last);

void			free_chain(t_token *start, t_token *end);

int				check_ambig(t_token *prev, int cnt, t_token *cur);

/** @endcond */

#endif
