/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:14 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/19 07:26:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//==================== FUNCTION: open_redir_file =========================
//
// PURPOSE:
//    Open file for redirection based on type
//
// RETURN:
//    int - File descriptor or -1 on error
//
// PARAMETERS:
//    char *file         - Filename to open
//    t_node_type type   - Type of redirection
//
// VARIABLES:
//    int fd - File descriptor
//
// ALGORITHM:
//    1. Check redirection type:
//       - REDIR_IN: open read-only
//       - REDIR_OUT: open write, create, truncate
//       - APPEND: open write, create, append
//       - HEREDOC: return heredoc fd (already open)
//    2. If open fails, print error and return -1
//    3. Return file descriptor
//
// PSEUDO-CODE:
//    if type == REDIR_IN:
//        fd = open(file, O_RDONLY)
//    if type == REDIR_OUT:
//        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)
//    if type == APPEND:
//        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)
//
//    if fd == -1:
//        print error
//    return fd
//
// EXAMPLE:
//    open_redir_file("output.txt", NODE_REDIR_OUT)
//    → opens output.txt for writing
//    → returns fd 3

int	open_redir_file(char *file, t_node_type type, int quoted, t_env *env)
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
		fd = handle_heredoc(file, quoted, env);
	if (fd == -1 && type != NODE_REDIR_HEREDOC)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
	}
	return (fd);
}

//==================== FUNCTION: setup_redirection =======================
//
// PURPOSE:
//    Setup file descriptor redirection (stdin or stdout)
//
// RETURN:
//    int - Original fd (to restore later) or -1 on error
//
// PARAMETERS:
//    int fd             - File descriptor to redirect to
//    t_node_type type   - Redirection type
//
// VARIABLES:
//    int saved_fd - Original stdin/stdout to restore
//    int target   - STDIN or STDOUT constant
//
// ALGORITHM:
//    1. Determine target: STDIN for <, STDOUT for '>/>>'
//    2. Save original fd with dup()
//    3. Redirect with dup2(fd, target)
//    4. Close the file fd (now duplicated)
//    5. Return saved_fd
//
// PSEUDO-CODE:
//    if type == REDIR_IN:
//        target = STDIN
//    else:
//        target = STDOUT
//
//    saved_fd = dup(target)
//    dup2(fd, target)
//    close(fd)
//    return saved_fd
//
// EXAMPLE:
//    fd = open("input.txt", O_RDONLY)  // fd = 3
//    saved = setup_redirection(3, REDIR_IN)
//    → dup(STDIN) = 4 (saved)
//    → dup2(3, STDIN) → stdin now reads from file
//    → close(3)
//    → returns 4 (to restore later)

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

//==================== FUNCTION: restore_fd ==============================
//
// PURPOSE:
//    Restore original stdin/stdout after redirection
//
// RETURN:
//    void
//
// PARAMETERS:
//    int saved_fd       - Original fd to restore
//    t_node_type type   - Redirection type
//
// VARIABLES:
//    int target - STDIN or STDOUT to restore
//
// ALGORITHM:
//    1. Determine target based on type
//    2. Restore with dup2(saved_fd, target)
//    3. Close saved_fd
//
// PSEUDO-CODE:
//    if type == REDIR_IN:
//        target = STDIN
//    else:
//        target = STDOUT
//
//    dup2(saved_fd, target)
//    close(saved_fdfd)

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

//==================== FUNCTION: handle_redir ============================
//
// PURPOSE:
//    Execute a redirection node
//
// RETURN:
//    int - Exit status from command
//
// PARAMETERS:
//    t_ast *node    - REDIRECTION node
//    t_shell *shell - Shell state
//
// VARIABLES:
//    int fd         - File descriptor for redirection
//    int saved_fd   - Original stdin/stdout
//    int status     - Exit status from command
//
// ALGORITHM:
//    1. Open file for redirection
//    2. If open fails, return 1
//    3. Setup redirection (save original fd)
//    4. Recursively execute the command
//    5. Restore original fd
//    6. Return status
//
// PSEUDO-CODE:
//    fd = open_redir_file(node.data.redir.file, node.type)
//    if fd == -1:
//        return 1
//
//    saved_fd = setup_redirection(fd, node.type)
//    status = executor(node.data.redir.cmd, shell)  // RECURSIVE
//    restore_fd(saved_fd, node.type)
//
//    return status
//
// EXAMPLE:
//    Tree: REDIR_OUT(file="out.txt", cmd=COMMAND(echo hello))
//    → Opens out.txt
//    → Redirects stdout to file
//    → Executes echo hello (outputs to file)
//    → Restores stdout
//    → Returns 0

int	handle_redir(t_ast *node, t_shell *shell)
{
	t_redir_node	*redir;
	int				fd;
	int				saved_fd;
	int				status;

	redir = &node->data.redir;
	fd = open_redir_file(redir->file, node->type, redir->quote, shell->env);
	if (fd == -1)
		return (1);
	saved_fd = setup_redirection(fd, node->type);
	if (saved_fd == -1)
		return (1);
	status = executor(redir->cmd, shell);
	restore_fd(saved_fd, node->type);
	return (status);
}
