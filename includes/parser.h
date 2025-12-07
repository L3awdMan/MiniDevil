/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:17:12 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/07 22:27:43 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"
# include "token.h"

// Main entry
t_ast	*parse(t_token *tokens);

// Pipeline parsing
t_ast	*parse_pipeline(t_token **tokens);

// Command parsing
t_ast	*parse_simple_command(t_token **tokens);
char	**collect_args(t_token **tokens, int *argc);

#endif
