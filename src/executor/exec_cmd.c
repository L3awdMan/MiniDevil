/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 23:16:35 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:20:11 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_exec_error(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	exit(126);
}

static void	child_execute(char *path, char **args, char **envp)
{
	reset_child_signals();
	if (execve(path, args, envp) == -1)
		handle_exec_error(path);
}

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

int	exec_external(char **args, t_env *env)
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (0);
	status = prepare_exec(args, env, &path, &envp);
	if (status != 0)
		return (status);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (free(path), ft_free_strarray(envp), 1);
	}
	if (pid == 0)
		child_execute(path, args, envp);
	status = wait_for_child(pid);
	free(path);
	ft_free_strarray(envp);
	return (status);
}
