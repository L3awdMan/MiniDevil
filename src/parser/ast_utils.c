/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:02:54 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/21 08:30:24 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "structs.h"

//===================== FUNCTION: free_args ==============================
	//
	// PURPOSE:
	//    Free array of strings (command arguments)
	//
	// RETURN:
	//    void
	//
	// PARAMETERS:
	//    char **args - Array of strings to free
	//
	// VARIABLES:
	//    int i - Loop counter
	//
	// ALGORITHM:
	//    1. Check if args is NULL
	//    2. Loop through each string
	//    3. Free each string
	//    4. Free the array itself
	//
	// EXAMPLE:
	//    args = ["echo", "hello", NULL]
	//    free_args(args)
	//    Result: All memory freed

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

//===================== FUNCTION: free_ast ===============================
	//
	// PURPOSE:
	//    Recursively free entire AST tree
	//
	// RETURN:
	//    void
	//
	// PARAMETERS:
	//    t_ast *node - Root node of tree to free
	//
	// VARIABLES:
	//    None
	//
	// ALGORITHM:
	//    1. If node is NULL: return
	//    2. Check node type:
	//       a. COMMAND: free args array
	//       b. PIPE: recursively free left and right
	//       c. REDIR: free file and recursively free cmd
	//    3. Free the node itself
	//
	// EXAMPLE:
	//    tree = pipe(cmd("echo"), cmd("cat"))
	//    free_ast(tree)
	//    Result: Entire tree freed

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
		free_args(node->data.cmd.args);
	else if (node->type == NODE_PIPE)
	{
		free_ast(node->data.binary.left);
		free_ast(node->data.binary.right);
	}
	else if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
	{
		if (node->data.redir.heredoc_fd >= 0)
			close(node->data.redir.heredoc_fd);
		free(node->data.redir.file);
		free_ast(node->data.redir.cmd);
	}
	free(node);
}
