/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:42:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/22 01:10:07 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "structs.h"

//===================== FUNCTION: create_cmd_node ========================
	//
	// PURPOSE:
	//    Create a command node in the AST
	//    Represents simple command with arguments
	//
	// RETURN:
	//    t_ast * - New command node, NULL on malloc error
	//
	// PARAMETERS:
	//    char **args - Array of arguments ["echo", "hello", NULL]
	//    int argc    - Number of arguments (2)
	//
	// VARIABLES:
	//    t_ast *node - The new node to create
	//
	// ALGORITHM:
	//    1. Allocate memory for new AST node
	//    2. Check if malloc succeeded
	//    3. Set type to NODE_COMMAND
	//    4. Store args and argc in cmd data
	//    5. Return the node
	//
	// EXAMPLE:
	//    args = ["echo", "hello", NULL]
	//    node = create_cmd_node(args, 2)
	//    Result: Command node for "echo hello"

t_ast	*create_cmd_node(char **args, int argc)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->data.cmd.args = args;
	node->data.cmd.argc = argc;
	return (node);
}

//===================== FUNCTION: create_pipe_node =======================
	//
	// PURPOSE:
	//    Create a pipe node connecting two commands
	//    Represents: left | right
	//
	// RETURN:
	//    t_ast * - New pipe node, NULL on malloc error
	//
	// PARAMETERS:
	//    t_ast *left  - Left command (before pipe)
	//    t_ast *right - Right command (after pipe)
	//
	// VARIABLES:
	//    t_ast *node - The new node to create
	//
	// ALGORITHM:
	//    1. Allocate memory for new AST node
	//    2. Check if malloc succeeded
	//    3. Set type to NODE_PIPE
	//    4. Store left command in binary.left
	//    5. Store right command in binary.right
	//    6. Return the node
	//
	// EXAMPLE:
	//    left = cmd("echo hello")
	//    right = cmd("cat")
	//    node = create_pipe_node(left, right)
	//    Result: Pipe node for "echo hello | cat"

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->data.binary.left = left;
	node->data.binary.right = right;
	return (node);
}

//==================== FUNCTION: create_redir_node =======================
	//
	// PURPOSE:
	//    Create a redirection node
	//    Represents: cmd < file, cmd > file, cmd >> file, cmd << EOF
	//
	// RETURN:
	//    t_ast * - New redirection node, NULL on malloc error
	//
	// PARAMETERS:
	//    t_node_type type - Redirection type (IN/OUT/APPEND/HEREDOC)
	//    char *file       - Filename for redirection
	//    t_ast *cmd       - Command to redirect
	//
	// VARIABLES:
	//    t_ast *node - The new node to create
	//
	// ALGORITHM:
	//    1. Allocate memory for new AST node
	//    2. Check if malloc succeeded
	//    3. Set type to specified redirection type
	//    4. Store filename in redir.file
	//    5. Store command in redir.cmd
	//    6. Store redir_type
	//    7. Return the node
	//
	// EXAMPLE:
	//    cmd = cmd_node("cat")
	//    node = create_redir_node(NODE_REDIR_IN, "input.txt", cmd)
	//    Result: Redirection node for "cat < input.txt"

t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd, int quote)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->data.redir.file = file;
	node->data.redir.cmd = cmd;
	node->data.redir.redir_type = type;
	node->data.redir.quote = quote;
	node->data.redir.heredoc_fd = -1;
	return (node);
}
