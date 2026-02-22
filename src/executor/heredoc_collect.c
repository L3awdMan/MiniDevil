/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 07:43:01 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/22 01:08:33 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Walk the entire AST before execution and prefill every heredoc pipe
 * in left -> right order (like bash)
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

static void	close_heredoc_fds(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		close_heredoc_fds(node->data.binary.left);
		close_heredoc_fds(node->data.binary.right);
	}
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

int	walk_heredocs(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		if (walk_heredocs(node->data.binary.left, shell) == -1)
			return (-1);
		return (walk_heredocs(node->data.binary.right, shell));
	}
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
		return (walk_redir(node, shell));
	return (0);
}

int	collect_heredocs(t_ast *node, t_shell *shell)
{
	if (walk_heredocs(node, shell) == -1)
	{
		close_heredoc_fds(node);
		return (-1);
	}
	return (0);
}
