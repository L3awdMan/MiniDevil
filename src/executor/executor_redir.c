/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:14 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:50:27 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include "executor.h"
#include "libft.h"

/**
 * @brief Open a file for redirection (based on the redirection type)
 * 
 * Opens the file with appropriate flags:
 * - `<` O_RDONLY for input
 * - `>` O_WRONLY | O_CREAT | O_TRUNC for output
 * - `>>` O_WRONLY | O_CREAT | O_APPEND for append
 * 
 * @param file Filename to open
 * @param type Redirection node type
 * @return File descriptor on success or -1 on failure
 */
int	open_redir_file(char *file, t_node_type type)
{
	int	fd;

	fd = -1;
	if (type == NODE_REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (type == NODE_REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == NODE_REDIR_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
	}
	return (fd);
}

/**
 * @brief Redirect STDIN or STDOUT to fd, saving the original
 * 
 * - Saves the original fd via dup and replaces it with dup2
 * - Closes the source fd after duplication
 * 
 * @param fd File descriptor to redirect to
 * @param type Redirection type (to determine STDIN or STDOUT target)
 * @return Saved original fd or -1 on error
 */
int	setup_redirection(int fd, t_node_type type)
{
	int	saved_fd;
	int	target;

	if (type == NODE_REDIR_IN || type == NODE_REDIR_HEREDOC)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	saved_fd = dup(target);
	if (saved_fd == -1)
	{
		perror("minishell: error on dup");
		return (-1);
	}
	if (dup2(fd, target) == -1)
	{
		close(saved_fd);
		perror("minishell: error on dup2");
		return (-1);
	}
	close(fd);
	return (saved_fd);
}

/**
 * @brief Restore a previously saved file descriptor
 * 
 * @param saved_fd Saved fd from setup_redirection()
 * @param type Redirection type
 */
void	restore_fd(int saved_fd, t_node_type type)
{
	int	target;

	if (saved_fd == -1)
		return ;
	if (type == NODE_REDIR_IN || type == NODE_REDIR_HEREDOC)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	dup2(saved_fd, target);
	close(saved_fd);
}

/**
 * @brief Execute a redirection node
 * 
 * - Opens the redirection target (file or heredoc)
 * - Saves the original fd
 * - Executes the child command with the redirected fd
 * - Restores the original fd
 * 
 * @param node AST redirection node
 * @param shell Shell context
 * @return Exit status of the child command or 1 on error
 * @note For precollected heredocs, uses the cached heredoc_fd
 */
int	handle_redir(t_ast *node, t_shell *shell)
{
	t_redir_node	*redir;
	int				fd;
	int				saved_fd;
	int				status;

	redir = &node->data.redir;
	if (node->type == NODE_REDIR_HEREDOC && redir->heredoc_fd >= 0)
	{
		fd = redir->heredoc_fd;
		redir->heredoc_fd = -1;
	}
	else
		fd = open_redir_file(redir->file, node->type);
	if (fd == -1)
		return (1);
	saved_fd = setup_redirection(fd, node->type);
	if (saved_fd == -1)
	{
		close(fd);
		return (1);
	}
	status = executor(redir->cmd, shell);
	restore_fd(saved_fd, node->type);
	return (status);
}
