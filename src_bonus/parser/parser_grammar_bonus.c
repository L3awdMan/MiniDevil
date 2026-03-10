/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_grammar_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:11:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 07:57:00 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * @brief Validate && and || positions
 * 
 * - Not at start
 * - Not at end
 * - Not consecutive
 * 
 * @param tokens Token list
 * @return 0 if valid and -1 on error
 */
static int	validate_logic_ops(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR)
		return (print_logic_error(tokens->type));
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_AND || current->type == TOKEN_OR)
		{
			if (!current->next)
				return (print_logic_error(current->type));
			if (current->next->type == TOKEN_AND
				|| current->next->type == TOKEN_OR)
				return (print_logic_error(current->type));
			if (current->next->type == TOKEN_RPAREN)
				return (print_syntax_error(ERR_PAREN_CLOSE));
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Validate parentheses are balanced and not empty
 * 
 * Also reject `)` immediately followed by a word or `(`
 * 
 * @param tokens Token list
 * @return 0 if valid and -1 if error
 */
static int	validate_parentheses(t_token *tokens)
{
	t_token	*current;
	int		count;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_LPAREN)
		{
			count++;
			if (current->next && current->next->type == TOKEN_RPAREN)
				return (print_syntax_error(ERR_PAREN_EMPTY));
		}
		if (current->type == TOKEN_RPAREN && --count < 0)
			return (print_syntax_error(ERR_PAREN_CLOSE));
		if (current->type == TOKEN_RPAREN && current->next
			&& (current->next->type == TOKEN_WORD
				|| current->next->type == TOKEN_LPAREN))
			return (print_syntax_error(ERR_UNEXPECTED_LPAREN));
		current = current->next;
	}
	if (count != 0)
		return (print_syntax_error(ERR_PAREN_OPEN));
	return (0);
}

/**
 * @brief Validate pipe positions (not at start nor end and no consecutive pipe)
 * 
 * @param tokens Token list
 * @return 0 if valid and -1 on error
 */
static int	validate_pipe_position(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
		return (print_syntax_error(ERR_PIPE_START));
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
				return (print_syntax_error(ERR_PIPE_END));
			if (current->next->type == TOKEN_PIPE)
				return (print_syntax_error(ERR_PIPE_DOUBLE));
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Validate that every redirection is followed by a word token
 * 
 * @param tokens Token list
 * @return 0 if valid and -1 on error
 */
static int	validate_redir_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (!current->next)
				return (print_syntax_error(ERR_REDIR_NO_FILE));
			if (current->next->type != TOKEN_WORD)
				return (print_syntax_error(ERR_REDIR_NO_FILE));
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Run all validation checks on a token list
 * 
 * @param tokens Token list to validate
 * @return 0 if valid and -1 on error (with message printed to STDERR)
 */
int	validate_syntax(t_token *tokens)
{
	if (!tokens)
		return (print_syntax_error(ERR_EMPTY_INPUT));
	if (validate_pipe_position(tokens) < 0)
		return (-1);
	if (validate_redir_syntax(tokens) < 0)
		return (-1);
	if (validate_logic_ops(tokens) < 0)
		return (-1);
	if (validate_parentheses(tokens) < 0)
		return (-1);
	return (0);
}
