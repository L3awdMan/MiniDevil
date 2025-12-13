/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:11:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/13 16:00:30 by zotaj-di         ###   ########.fr       */
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

//================== FUNCTION: collect_and_merge_remaining_argc ===============
//
// PURPOSE:
//    After parsing redirection, collect any remaining WORD tokens
//    and merge them into the command's argument array
//
// RETURN:
//    void
//
// PARAMETERS:
//    t_ast *cmd       - Command node to update (must be NODE_COMMAND)
//    t_token **tokens - Current token position
//
// VARIABLES:
//    char **new_args - Remaining arguments collected
//    char **merged   - New merged array containing all arguments
//    int new_argc    - Count of new arguments
//    int i           - Loop counter
//
// ALGORITHM:
//    1. Call collect_args() to get remaining WORD tokens
//    2. If no new args, return immediately
//    3. Allocate merged array: (old argc + new argc + 1) size
//    4. If malloc fails, return
//    5. Copy all old args pointers into merged (first section)
//    6. Copy all new args pointers into merged (second section)
//    7. Set NULL terminator at end
//    8. Free old args array (not the strings!)
//    9. Free new_args array (not the strings!)
//    10. Update cmd node

void	collect_and_merge_remaining_argc(t_ast *cmd, t_token **tokens)
{
	char	**new_args;
	char	**merged;
	int		new_argc;
	int		i;

	new_args = collect_args(tokens, &new_argc);
	if (!new_args)
		return ;
	merged = malloc(sizeof(char *) * (cmd->data.cmd.argc + new_argc + 1));
	if (!merged)
		return ;
	i = -1;
	while (++i < cmd->data.cmd.argc)
		merged[i] = cmd->data.cmd.args[i];
	i = -1;
	while (++i < new_argc)
		merged[cmd->data.cmd.argc + i] = new_args[i];
	merged[cmd->data.cmd.argc + new_argc] = NULL;
	free(cmd->data.cmd.args);
	free(new_args);
	cmd->data.cmd.args = merged;
	cmd->data.cmd.argc += new_argc;
}

//==================== FUNCTION: get_command_node ========================
//
// PURPOSE:
//    Extract the actual COMMAND node from nested redirections
//
// RETURN:
//    t_ast * - The innermost COMMAND node
//
// PARAMETERS:
//    t_ast *node - Node to search (could be REDIR or COMMAND)
//
// ALGORITHM:
//    1. While node exists and is a redirection type
//    2. Dig deeper: node = node->data.redir.cmd
//    3. Return the COMMAND node we found

static t_ast	*get_command_node(t_ast *node)
{
	while (node && node->type >= NODE_REDIR_IN
		&& node->type <= NODE_REDIR_HEREDOC)
		node = node->data.redir.cmd;
	return (node);
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
	t_ast	*cmd_node;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = parse_simple_command(tokens); // collects only argc before operator
	if (!cmd)
		return (NULL);
	while (*tokens && is_redirection((*tokens)->type))
	{
		cmd = parse_one_redirection(tokens, cmd);
		if (!cmd)
			return (NULL);
		cmd_node = get_command_node(cmd);
		collect_and_merge_remaining_argc(cmd_node, tokens);
	}
	return (cmd);
}
