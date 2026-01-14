/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/01/14 17:05:29 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//================== FUNCTION: expand_arg ================================
//
// PURPOSE:
//    just to expand variables in a single argument string
//
// RETURN:
//    char * - Expanded string (newly allocated)
//
// PARAMETERS:
//    char *arg       - Original argument
//    t_shell *shell  - Shell state (for env and exit_status)
//
// ALGORITHM:
//    1. Call expand_variables() with QUOTE_NONE context
//    2. Return expanded result

static char	*expand_arg(char *arg, t_shell *shell)
{
	return (expand_variables(arg, shell->env, QUOTE_NONE, shell->exit_status));
}

//================== FUNCTION: expand_args ===============================
//
// PURPOSE:
//    Expand variables in all command arguments
//
// RETURN:
//    char ** - New argument array with expansions (NULL on error)
//
// PARAMETERS:
//    char **args     - Original arguments array
//    t_shell *shell  - Shell state
//
// VARIABLES:
//    char **expanded - New array for expanded args
//    int i           - Loop counter
//
// ALGORITHM:
//    1. Count arguments
//    2. Allocate new array (same size + 1 for NULL)
//    3. Loop through each argument:
//       - Expand variables
//       - Store in new array
//    4. NULL-terminate new array
//    5. Return new array

char	**expand_args(char **args, t_shell *shell)
{
	char	**expanded;
	int		i;

	if (!args)
		return (NULL);
	expanded = malloc(sizeof(char *) * (ft_arrlen(args) + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded[i] = expand_arg(args[i], shell);
		if (!expanded[i])
		{
			ft_free_strarray(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

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
	char	**expanded_args;
	int		status;

	args = node->data.cmd.args;
	expanded_args = expand_args(args, shell);
	if (!expanded_args)
		return (1);
	status = exec_simple_command(expanded_args, &shell->env,
			shell->exit_status);
	ft_free_strarray(expanded_args);
	return (status);
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
