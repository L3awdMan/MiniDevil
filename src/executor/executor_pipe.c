/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:25:03 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:50:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include "executor.h"
#include "libft.h"
#include "ast.h"
#include "env.h"
#include "signals.h"
#include "get_next_line.h"

/**
 * @brief Execute the left side of a pipe in a child process
 * 
 * Closes the read end of the pipe and redirects STDOUT to the write end
 * 
 * @param left Left AST subtree to execute
 * @param pipe_fd Pipe file descriptors: [read----write]
 * @param shell Shell context
 * @note Never returns (calls exit)
 */
void	exec_left_pipe_child(t_ast *left, int pipe_fd[2], t_shell *shell)
{
	int	status;

	reset_child_signals();
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	shell->is_child = 1;
	status = executor(left, shell);
	free(shell->current_input);
	free_ast(shell->current_ast);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

/**
 * @brief Execute the right side of a pipe in a child process
 * 
 * Closes the write end of the pipe and redirects STDIN to the read end
 * 
 * @param right Right AST subtree to execute
 * @param pipe_fd Pipe file descriptors: [read----write]
 * @param shell Shell context
 * @note Never returns (calls exit)
 */
void	exec_right_pipe_child(t_ast *right, int pipe_fd[2], t_shell *shell)
{
	int	status;

	reset_child_signals();
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	shell->is_child = 1;
	status = executor(right, shell);
	free(shell->current_input);
	free_ast(shell->current_ast);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

/**
 * @brief Wait for both pipe children and return the right one's status
 * 
 * Handles signal deaths (SIGINT = newline, SIGQUIT = core dump message)
 * 
 * @param left_pid PID of the left pipe child
 * @param right_pid PID of the right pipe child
 * @return Exit status of the right child (0-255)
 */
static int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid)
{
	int	status;
	int	right_status;

	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		right_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		right_status = 128 + WTERMSIG(status);
	}
	else
		right_status = 1;
	return (right_status);
}

/**
 * @brief Execute a pipe node by forking left and right children
 * 
 * Creates a pipe, forks 2 children:
 * - Left child writes to pipe and right reads from it
 * - Parent closes both pipe ends & waits for both children
 * 
 * @param node AST pipe node with binary.left and binary.right subtrees
 * @param shell Shell context
 * @return Exit status of the rightmost command in the pipe
 */
int	handle_pipe(t_ast *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (pipe_fork_error(pipe_fd, 0));
	if (left_pid == 0)
		exec_left_pipe_child(node->data.binary.left, pipe_fd, shell);
	right_pid = fork();
	if (right_pid == -1)
		return (pipe_fork_error(pipe_fd, left_pid));
	if (right_pid == 0)
		exec_right_pipe_child(node->data.binary.right, pipe_fd, shell);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_for_pipe_children(left_pid, right_pid));
}
