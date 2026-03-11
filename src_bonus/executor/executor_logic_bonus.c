/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_logic_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 06:45:02 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/09 07:21:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include "executor.h"
#include "ast.h"
#include "env.h"
#include "signals.h"
#include "get_next_line.h"
#include "libft.h"

/**
 * @brief Execute a AND / OR node
 * 
 * Runs the left first then runs the right side accordingly.
 * - AND skips right if left failed
 * - OR skips right if left succeeded
 * 
 * @param node Binary AST node (NODE_AND or NODE_OR)
 * @param shell Shell context
 * @return Exit status of the last executed side
 * @note Updates shell->exit_status after the left side so builtins like export
 * in the left side take effect before the right side runs
 */
int	handle_and_or(t_ast *node, t_shell *shell)
{
	int	status;

	status = executor(node->data.binary.left, shell);
	shell->exit_status = status;
	if (node->type == NODE_AND && status != 0)
		return (status);
	if (node->type == NODE_OR && status == 0)
		return (status);
	status = executor(node->data.binary.right, shell);
	shell->exit_status = status;
	return (status);
}

/**
 * @brief Child process body in a subshell
 * 
 * Reset signals, execute the inner AST and clean everything up then exit
 * 
 * @param node Subshell AST node
 * @param shell Shell context
 * @note shell->is_child = 0 is critical because it tells exec_external() to
 * fork again for every external command inside this subshell, instead of
 * replacing the process with execve(). Without this, something like
 * `(sleep 10 && echo done) would exec sleep and never reach echo
 */
static void	subshell_child(t_ast *node, t_shell *shell)
{
	int	status;

	reset_child_signals();
	shell->is_child = 0;
	status = executor(node->data.subshell.child, shell);
	free(shell->current_input);
	free_ast(shell->current_ast);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

/**
 * @brief Wait for a subshell child & return its exit status
 * 
 * @param pid PID of the child to wait for
 * @return Exit code (0-255) or 128 + the signal on signal term
 */
static int	wait_subshell(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Fork and execute a subshell
 * 
 * @param node Subshell AST node
 * @param shell Shell context
 * @return Exit status of the subshell child
 */
int	handle_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		subshell_child(node, shell);
	return (wait_subshell(pid));
}
