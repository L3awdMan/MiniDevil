/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:11:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 00:00:44 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

//==================== FUNCTION: is_redirection ==========================
//
// PURPOSE:
//    Check if token is a redirection operator
//
// RETURN:
//    int - 1 if redirection, 0 otherwise
//
// PARAMETERS:
//    t_token_type type - Token type to check
//
// ALGORITHM:
//    Return true if type is any of the 4 redirection types

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

//==================== FUNCTION: validate_empty ==========================
//
// PURPOSE:
//    Check for empty input
//
// RETURN:
//    int - 0 if valid (not empty), -1 if invalid (empty)
//
// PARAMETERS:
//    t_token *tokens - Token list to validate
//
// ALGORITHM:
//    If tokens is NULL, print error and return -1

int	validate_empty(t_token *tokens)
{
	if (!tokens)
	{
		write(2, "syntax error: empty input\n", 26);
		return (-1);
	}
	return (0);
}

//==================== FUNCTION: validate_pipe_position ==================
//
// PURPOSE:
//    Check for pipes at start or end
//
// RETURN:
//    int - 0 if valid, -1 if invalid
//
// PARAMETERS:
//    t_token *tokens - Token list to validate
//
// ALGORITHM:
//    1. Check if first token is pipe
//    2. Find last token and check if it's pipe
//    3. Return -1 if either case found

int	validate_pipe_position(t_token *tokens)
{
	t_token	*current;

	if (tokens->type == TOKEN_PIPE)
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (-1);
	}
	current = tokens;
	while (current->next)
		current = current->next;
	if (current->type == TOKEN_PIPE)
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (-1);
	}
	return (0);
}

//==================== FUNCTION: validate_consecutive ====================
//
// PURPOSE:
//    Check for consecutive operators without words between
//
// RETURN:
//    int - 0 if valid, -1 if invalid
//
// PARAMETERS:
//    t_token *tokens - Token list to validate
//
// ALGORITHM:
//    Loop through tokens and check for:
//    - Pipe followed by pipe
//    - Pipe followed by redirection
//    - Redirection followed by non-word

int	validate_consecutive(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_PIPE && current->next->type == TOKEN_PIPE)
		{
			write(2, "syntax error near unexpected token `|'\n", 39);
			return (-1);
		}
		if (current->type == TOKEN_PIPE && is_redirection(current->next->type))
		{
			write(2, "syntax error: missing command after pipe\n", 41);
			return (-1);
		}
		if (is_redirection(current->type) && current->next->type != TOKEN_WORD)
		{
			write(2, "syntax error: missing filename after redirection\n", 49);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

//==================== FUNCTION: validate_redirection_file ===============
//
// PURPOSE:
//    Check that redirections have filenames
//
// RETURN:
//    int - 0 if valid, -1 if invalid
//
// PARAMETERS:
//    t_token *tokens - Token list to validate
//
// ALGORITHM:
//    Find last token, check if it's a redirection

int	validate_redirection_file(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current->next)
		current = current->next;
	if (is_redirection(current->type))
	{
		write(2, "syntax error: missing filename after redirection\n", 49);
		return (-1);
	}
	return (0);
}

//==================== FUNCTION: validate_syntax =========================
//
// PURPOSE:
//    Main validation function - checks all syntax rules
//
// RETURN:
//    int - 0 if valid, -1 if invalid
//
// PARAMETERS:
//    t_token *tokens - Token list to validate
//
// ALGORITHM:
//    Run all validation checks in sequence
//    Return -1 if any check fails
//
// EXAMPLE:
//    "| cat"     → validate_pipe_position fails
//    "cat |"     → validate_pipe_position fails
//    "cat <"     → validate_redirection_file fails
//    "cat | | grep" → validate_consecutive fails

int	validate_syntax(t_token *tokens)
{
	if (validate_empty(tokens) < 0)
		return (-1);
	if (validate_pipe_position(tokens) < 0)
		return (-1);
	if (validate_consecutive(tokens) < 0)
		return (-1);
	if (validate_redirection_file(tokens) < 0)
		return (-1);
	return (0);
}
