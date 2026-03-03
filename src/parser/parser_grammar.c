/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_grammar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:11:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 07:47:37 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "structs.h"

/**
 * @brief Validate that the token list isn't empty
 * 
 * @param tokens Token list
 * @return 0 if not empty and -1 on error
 */
static int	validate_empty(t_token *tokens)
{
	if (!tokens)
		return (print_syntax_error(ERR_EMPTY_INPUT));
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
 * @brief Validate that pipes are not followed by any redirection
 * 
 * @param tokens Token list
 * @return 0 if valid and -1 on error
 */
static int	validate_pipe_context(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (is_redirection(current->next->type))
				return (print_syntax_error(ERR_PIPE_NO_CMD));
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
	if (validate_empty(tokens) < 0)
		return (-1);
	if (validate_pipe_position(tokens) < 0)
		return (-1);
	if (validate_pipe_context(tokens) < 0)
		return (-1);
	if (validate_redir_syntax(tokens) < 0)
		return (-1);
	return (0);
}
