/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/17 19:53:55 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//==================== FUNCTION: exec_command_node ====================
//
// PURPOSE:
//    Execute a COMMAND node (base case of recursion)
//    This is where actual command execution happens
//
// RETURN:
//    int - Exit status of the command
//
// PARAMETERS:
//    t_ast *node      - The COMMAND node to execute
//    t_shell *shell   - Shell state (env, exit_status)
//
// VARIABLES:
//   char **args
//
// ALGORITHM:
//    1. Extract args from node->data.cmd.args
//    2. Call exec_simple_command (handles builtin vs external)
//    3. Return the exit status
//
// PSEUDO-CODE:
//    args = node.data.cmd.args
//    status = exec_simple_command(args, &shell.env, shell.exit_status)
//    return status
//
// EXAMPLE:
//    node = COMMAND(args=["echo", "hello"])
//    → calls exec_simple_command
//    → returns 0 (success)

static int	exec_command_node(t_ast *node, t_shell *shell)
{
	char	**args;

	args = node->data.cmd.args;
	return (exec_simple_command(args, &shell->env, shell->exit_status));
}

//==================== FUNCTION: executor =============================
//
// PURPOSE:
//    Main AST executor - recursive tree traversal dispatcher
//    Routes each node type to its appropriate handler
//
// RETURN:
//    int - Exit status from command execution
//
// PARAMETERS:
//    t_ast *node    - Current AST node to execute
//    t_shell *shell - Shell state
//
// VARIABLES:
//    None
//
// ALGORITHM:
//    1. BASE CASE: If node is NULL, return 0
//    2. Check node type:
//       a. NODE_COMMAND → execute_command_node()
//       b. NODE_PIPE → handle_pipe()
//       c. NODE_REDIR_* → handle_redir()
//    3. Return the status from handler
//
// PSEUDO-CODE:
//    if node == NULL:
//        return 0
//
//    if node.type == COMMAND:
//        return execute_command_node(node, shell)
//
//    if node.type == PIPE:
//        return handle_pipe(node, shell)
//
//    if node.type is any REDIRECTION:
//        return handle_redir(node, shell)
//
//    return 0
//
// EXAMPLE:
//    Tree: PIPE(CMD(echo), CMD(cat))
//    → Calls handle_pipe()
//    → handle_pipe recursively calls execute_ast() on children
//    → Returns final status

int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (exec_command_node(node, shell));
	if (node->type == NODE_PIPE)
		return (handle_pipe(node, shell));
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
		return (handle_redir(node, shell));
	return (0);
}
