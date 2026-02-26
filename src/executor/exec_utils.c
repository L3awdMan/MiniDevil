/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:16:34 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/26 02:17:37 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (127);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(args, &shell->env));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (builtin_export(args, &shell->env));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (builtin_unset(args, &shell->env));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (builtin_env(args, shell->env));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (builtin_exit(args, shell));
	return (0);
}

int	exec_simple_command(char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (0);
	if (is_builtin(args[0]))
		return (exec_builtin(args, shell));
	return (exec_external(args, shell));
}

int	pipe_fork_error(int pipe_fd[2], pid_t left_pid)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (left_pid > 0)
		waitpid(left_pid, NULL, 0);
	perror("minishell: fork");
	return (1);
}
