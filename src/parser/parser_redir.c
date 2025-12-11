/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:11:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 05:24:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

//==================== FUNCTION: get_redir_node_type =====================
	//
	// PURPOSE:
	//    Convert token type to AST node type for redirections
	//
	// RETURN:
	//    t_node_type - Corresponding AST node type
	//
	// PARAMETERS:
	//    t_token_type type - Token type
	//
	// ALGORITHM:
	//    Map TOKEN_REDIR_* to NODE_REDIR_*

static t_node_type	get_redir_node_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (NODE_REDIR_APPEND);
	if (type == TOKEN_HEREDOC)
		return (NODE_REDIR_HEREDOC);
	return (NODE_COMMAND);
}

//==================== FUNCTION: parse_one_redirection ===================
	//
	// PURPOSE:
	//    Parse a single redirection (operator + filename)
	//
	// RETURN:
	//    t_ast * - Redirection node wrapping the command, NULL on error
	//
	// PARAMETERS:
	//    t_token **tokens - Current token pointer (will be advanced)
	//    t_ast *cmd       - Command to wrap with redirection
	//
	// VARIABLES:
	//    t_node_type type - Redirection node type
	//    char *file       - Filename for redirection
	//
	// ALGORITHM:
	//    1. Get redirection type from current token
	//    2. Advance past operator token
	//    3. Check next token is WORD (filename)
	//    4. Duplicate filename
	//    5. Advance past filename token
	//    6. Create redirection node wrapping command
	//
	// EXAMPLE:
	//    tokens: [<, input.txt, ...]
	//    cmd: CMD(cat)
	//    Result: REDIR_IN(file="input.txt", cmd=CMD(cat))

static t_ast	*parse_one_redirection(t_token **tokens, t_ast *cmd)
{
	t_node_type	type;
	char		*file;

	type = get_redir_node_type((*tokens)->type);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (free_ast(cmd), NULL);
	file = ft_strdup((*tokens)->value);
	if (!file)
		return (free_ast(cmd), NULL);
	*tokens = (*tokens)->next;
	return (create_redir_node(type, file, cmd));
}

//==================== FUNCTION: parse_command ===========================
	//
	// PURPOSE:
	//    Parse command with all its redirections
	//    Handles: cat < in.txt > out.txt
	//
	// RETURN:
	//    t_ast * - Command node wrapped in redirection nodes, NULL on error
	//
	// PARAMETERS:
	//    t_token **tokens - Current token pointer (will be advanced)
	//
	// VARIABLES:
	//    t_ast *cmd - Command node being built
	//
	// ALGORITHM:
	//    1. Parse simple command (collects words)
	//    2. While current token is redirection:
	//       a. Parse the redirection
	//       b. Wrap command with new redirection node
	//    3. Return final wrapped command
	//
	// EXAMPLE:
	//    Input: cat < in.txt > out.txt
	//    Step 1: cmd = CMD(cat)
	//    Step 2: cmd = REDIR_IN(in.txt, CMD(cat))
	//    Step 3: cmd = REDIR_OUT(out.txt, REDIR_IN(in.txt, CMD(cat)))
	//    Result: Nested redirections around command

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = parse_simple_command(tokens);
	if (!cmd)
		return (NULL);
	while (*tokens && is_redirection((*tokens)->type))
	{
		cmd = parse_one_redirection(tokens, cmd);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}
