/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:14 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:20:52 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_file(char *file, t_node_type type, int quoted, t_shell *shell)
{
	int	fd;

	fd = -1;
	if (type == NODE_REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (type == NODE_REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == NODE_REDIR_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == NODE_REDIR_HEREDOC)
		fd = handle_heredoc(file, quoted, shell);
	if (fd == -1 && type != NODE_REDIR_HEREDOC)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
	}
	return (fd);
}

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
		return (perror("minishell: error on dup"), -1);
	if (dup2(fd, target) == -1)
	{
		close(saved_fd);
		return (perror("minishell: error on dup2"), -1);
	}
	close(fd);
	return (saved_fd);
}

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
		fd = open_redir_file(redir->file, node->type, redir->quote, shell);
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
