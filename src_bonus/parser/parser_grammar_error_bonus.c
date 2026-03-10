/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_grammar_error_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:18:08 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 07:58:08 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

/**
 * @brief Print a syntax error if && or || were misplaced
 * 
 * @param type TOKEN_AND or TOKEN_OR
 * @return -1 always
 */
int	print_logic_error(t_token_type type)
{
	if (type == TOKEN_AND)
		return (print_syntax_error(ERR_AND_UNEXPECTED));
	return (print_syntax_error(ERR_OR_UNEXPECTED));
}

/**
 * @brief Convert an error code to the corresponding message
 * 
 * @param err Error code
 * @return Static error message string or NULL if none were found
 */
static const char	*get_error_msg(t_syntax_error err)
{
	if (err == ERR_EMPTY_INPUT)
		return ("minishell: syntax error: empty input\n");
	if (err == ERR_PIPE_START || err == ERR_PIPE_END || err == ERR_PIPE_DOUBLE)
		return ("minishell: syntax error near unexpected token `|'\n");
	if (err == ERR_PIPE_NO_CMD)
		return ("minishell: syntax error: missing command after pipe\n");
	if (err == ERR_REDIR_NO_FILE)
		return ("minishell: syntax error: missing filename "
			"after redirection\n");
	if (err == ERR_REDIR_AFTER_PIPE)
		return ("minishell: syntax error: redirection after pipe\n");
	if (err == ERR_AND_UNEXPECTED)
		return ("minishell: syntax error near unexpected token `&&'\n");
	if (err == ERR_OR_UNEXPECTED)
		return ("minishell: syntax error near unexpected token `||'\n");
	if (err == ERR_PAREN_OPEN)
		return ("minishell: syntax error unmatched `('\n");
	if (err == ERR_PAREN_CLOSE || err == ERR_PAREN_EMPTY)
		return ("minishell: syntax error near unexpected token `)'\n");
	if (err == ERR_UNEXPECTED_LPAREN)
		return ("minishell: syntax error near unexpected token `('\n");
	return (NULL);
}

/**
 * @brief Print an error message to STDERR
 * 
 * @param err Error code
 * @return -1 always (value that will be return by callers)
 */
int	print_syntax_error(t_syntax_error err)
{
	const char	*msg;

	if (err == ERR_NONE)
		return (-1);
	msg = get_error_msg(err);
	if (msg)
		ft_putstr_fd((char *)msg, 2);
	return (-1);
}

/**
 * @brief Check if a token type is a redirection operator
 * 
 * @param type Token type to check
 * @return 1 if redirection and 0 if not
 */
int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}
