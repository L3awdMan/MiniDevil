/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:17:12 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 08:59:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

t_ast	*parse(t_token *tokens);

t_ast	*parse_pipeline(t_token **tokens);

t_ast	*parse_command(t_token **tokens);

t_ast	*parse_simple_command(t_token **tokens);

char	**collect_args(t_token **tokens, int *argc);

int		print_syntax_error(t_syntax_error err);

int		is_redirection(t_token_type type);

int		validate_syntax(t_token *tokens);

int		count_word_tokens(t_token *tokens);

t_ast	*reverse_redir_chain(t_ast *node);

char	*join_connected_delim(t_token **tokens, int *quoted);

#endif
