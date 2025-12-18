/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:17:12 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/18 15:39:41 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/**
 * @file parser.h
 * @brief Parser function prototypes
 *
 * The parser converts a token list into an AST using recursive descent
 *
 * For more details @see !wip
 * - A pipeline is one commande maybe followed by | and other commands
 * - A command is a list of words to which we can attach redirections
 * - A redirection is a redirection symbol followed by a filename or limiter
 */

# include "minishell.h"
# include "structs.h"

//==================================================
//================ INITIALIZATION ==================
//==================================================

/**
 * @brief Main parser
 *
 * Convert token list to AST
 *
 * @param tokens Token lists from tokenizer
 * @return AST Root node or NULL on parsing error
 */
t_ast	*parse(t_token *tokens);

//==================================================
//=============== PIPELINE PARSING =================
//==================================================

/**
 * @brief Parse a pipeline
 *
 * Handle cmd1 | cmd2 | cmd3
 * Use recursive descent
 *
 * @param tokens Pointer to current token (modified as tokens are processed)
 * @return Pipeline AST Node
 */
t_ast	*parse_pipeline(t_token **tokens);

//==================================================
//=============== COMMAND PARSING ==================
//==================================================

/**
 * @brief Parse a command with all its redirections
 *
 * Handle cmd arg1 < infile > outfile arg2
 * Wraps command in redirection nodes as needed
 *
 * @param tokens Pointer to current token (modified as tokens are processed)
 * @return Command AST node wrapped in redirection nodes, or NULL on error
 */
t_ast	*parse_command(t_token **tokens);

/**
 * @brief Parse a simple command with redirections
 *
 * Handle cmd1 arg1 arg2 < infile > outfile
 *
 * @param tokens Pointer to current token
 * @return Command AST node (could be wrapped in redirection nodes)
 */
t_ast	*parse_simple_command(t_token **tokens);

/**
 * @brief COllect word tokens into arguments array
 *
 * @param tokens Pointer to current token
 * @param argc Number of arguments collected
 * @return NULL terminated argument array
 */
char	**collect_args(t_token **tokens, int *argc);

//==================================================
//=============== GRAMMAR PARSING ==================
//==================================================

int		print_syntax_error(t_syntax_error err);
int		is_redirection(t_token_type type);
int		validate_syntax(t_token *tokens);

int		count_word_tokens(t_token *tokens);

#endif
