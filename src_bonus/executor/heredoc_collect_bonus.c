/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 07:43:01 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/06 07:03:45 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"

/**
 * @brief Collect a single heredoc content into a pipe fd
 * 
 * Calls handle_heredoc() to read the input and stores the result in the
 * node's heredoc_fd variable for later use during execution
 * 
 * @param node Heredoc redirection node (heredoc_fd is set on success)
 * @param shell Shell context
 * @return 0 on success and -1 on error or SIGINT
 */
static int	collect_one(t_ast *node, t_shell *shell)
{
	int	fd;

	fd = handle_heredoc(node->data.redir.file, node->data.redir.quote, shell);
	if (fd == -1)
		return (-1);
	node->data.redir.heredoc_fd = fd;
	return (0);
}

/**
 * @brief Process a redirection node (and collecting heredoc if applicable)
 * 
 * If the node is a heredoc, it collects its content, then recursively walks
 * into the inner command node to find more redirections
 * 
 * @param node Redirection AST node
 * @param shell Shell context
 * @return 0 on success and -1 if any heredoc collection fails
 */
static int	walk_redir(t_ast *node, t_shell *shell)
{
	if (node->type == NODE_REDIR_HEREDOC)
	{
		if (collect_one(node, shell) == -1)
			return (-1);
	}
	if (node->data.redir.cmd)
		return (walk_heredocs(node->data.redir.cmd, shell));
	return (0);
}

/**
 * @brief Recursively close cached heredoc file descriptors in the AST
 * 
 * @param node Root of the AST subtree
 */
static void	close_heredoc_fds(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE || node->type == NODE_AND
		|| node->type == NODE_OR)
	{
		close_heredoc_fds(node->data.binary.left);
		close_heredoc_fds(node->data.binary.right);
	}
	else if (node->type == NODE_SUBSHELL)
		close_heredoc_fds(node->data.subshell.child);
	else if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
	{
		if (node->type == NODE_REDIR_HEREDOC
			&& node->data.redir.heredoc_fd >= 0)
		{
			close(node->data.redir.heredoc_fd);
			node->data.redir.heredoc_fd = -1;
		}
		close_heredoc_fds(node->data.redir.cmd);
	}
}

/**
 * @brief Recursively walk the AST collecting all heredocs
 * 
 * It traverses the pipe & redirection nodes first to ensure all heredoc prompts
 * appear before any command runs
 * 
 * @param node Root of the AST Subtree
 * @param shell Shell context
 * @return 0 on success and -1 if a heredoc fails (SIGINT or error)
 */
int	walk_heredocs(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE || node->type == NODE_AND
		|| node->type == NODE_OR)
	{
		if (walk_heredocs(node->data.binary.left, shell) == -1)
			return (-1);
		return (walk_heredocs(node->data.binary.right, shell));
	}
	if (node->type == NODE_SUBSHELL)
		return (walk_heredocs(node->data.subshell.child, shell));
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
		return (walk_redir(node, shell));
	return (0);
}

/**
 * @brief Pre collect all heredocs in the AST before the execution phase
 * 
 * Walks the entire AST to read all heredoc inputs first. On failure it closes
 * any previously collected heredocs fds to prevent leaks
 * 
 * @param node Root of the AST
 * @param shell Shell context
 * @return 0 on success and -1 on failure
 */
int	collect_heredocs(t_ast *node, t_shell *shell)
{
	if (walk_heredocs(node, shell) == -1)
	{
		close_heredoc_fds(node);
		return (-1);
	}
	return (0);
}
