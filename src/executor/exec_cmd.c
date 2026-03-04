/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 23:16:35 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:48:51 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include "executor.h"
#include "libft.h"
#include "env.h"
#include "ast.h"
#include "signals.h"
#include "get_next_line.h"

/**
 * @brief Print an error for when execve fails
 * 
 * @param path Path that failed to execute
 */
static void	handle_exec_error(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
}

/**
 * @brief Replace current process with the external command (execve)
 * 
 * - Resets signals to default and calls execve
 * - On failure, prints an error, frees resources and exits with code 126
 * 
 * @param path Resolved path (free on failure)
 * @param args Argument array for execve
 * @param envp Environment array for execve (free on failure)
 * @param shell Shell context (free on failure)
 */
static void	child_execute(char *path, char **args, char **envp, t_shell *shell)
{
	reset_child_signals();
	if (execve(path, args, envp) == -1)
	{
		handle_exec_error(path);
		free(path);
		ft_free_strarray(envp);
		free(shell->current_input);
		free_ast(shell->current_ast);
		free_env_list(&shell->env);
		get_next_line(-42);
		exit(126);
	}
}

/**
 * @brief Wait for a child process and get the correct exit status
 * 
 * Handles normal exits (WEXITSTATUS) and signal deaths (128 + signal)
 * 
 * @param pid PID of the child to wait for
 * @return Correct exit code (0-255)
 */
static int	wait_for_child(pid_t pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		exit_code = 128 + WTERMSIG(status);
	}
	return (exit_code);
}

/**
 * @brief Resolve the command's path and build envp array before forking
 * 
 * Handles special cases: `.` and `..`
 * 
 * @param args Argument array with arg[0] being the command name
 * @param env Environment list (for PATH)
 * @param path resolved path
 * @param envp envp array
 * @return 0 on success or an error exit code (1, 2 or 127)
 */
static int	prepare_exec(char **args, t_env *env, char **path, char ***envp)
{
	if (ft_strncmp(args[0], ".", 2) == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n",
			STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		return (2);
	}
	if (ft_strncmp(args[0], "..", 3) == 0)
		return (exec_cmd_not_found(args[0]));
	*path = find_cmd_path(args[0], env);
	if (!*path)
		return (exec_cmd_not_found(args[0]));
	*envp = env_to_array(env);
	if (!*envp)
	{
		free(*path);
		return (1);
	}
	return (0);
}

/**
 * @brief Fork and execute an external command
 * 
 * Resolves the path, builds envp and forks a child that calls execve
 * - If already in a child process, executes directly without forking
 * - The parent waits for the child and returns its exit status code
 * 
 * @param args Argument array with arg[0] being the command name
 * @param shell Shell context
 * @return exit status (0 success, 126 exec error or 127 not found)
 */
int	exec_external(char **args, t_shell *shell)
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (0);
	status = prepare_exec(args, shell->env, &path, &envp);
	if (status != 0)
		return (status);
	if (shell->is_child)
		child_execute(path, args, envp, shell);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (free(path), ft_free_strarray(envp), 1);
	}
	if (pid == 0)
		child_execute(path, args, envp, shell);
	status = wait_for_child(pid);
	free(path);
	ft_free_strarray(envp);
	return (status);
}
