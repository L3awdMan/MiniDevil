/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:02:54 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:46:13 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"

/**
 * @brief Free a string array and its entries
 * 
 * @param args Array to free
 */
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

/**
 * @brief Recursively free an entire AST tree
 * 
 * - NODE_COMMAND: frees the args array
 * - NODE_PIPE: recurses into left and right children
 * - NODE_REDIR: closes heredoc_fd if >= 0, frees the filename and recurses into
 * the command subtree
 * 
 * @param node Root of tree to free
 */
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
