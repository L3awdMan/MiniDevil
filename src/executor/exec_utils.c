/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:16:34 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/14 05:32:10 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print command not found and return 127
 * 
 * @param cmd The command that was not found
 * @return 127
 */
int	exec_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (127);
}

/**
 * @brief Check if the command is a builtin
 * 
 * @param cmd The command name to check
 * @return 1 if builtin and 0 if not
 */
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

/**
 * @brief Execute a builtin command
 * 
 * Dispatche to the correct builtin function
 * @param args The command arguments
 * @param env Pointer to the environment list
 * @param last_status The last exit status (needed by the exit builtin)
 * @return Exit status of the builtin or 0
 */
int	exec_builtin(char **args, t_env **env, int last_status)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(args, env));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (builtin_export(args, env));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (builtin_unset(args, env));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (builtin_env(*env));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (builtin_exit(args, last_status));
	return (0);
}

/**
 * @brief Execute a simple command (can be builtin or external)
 * 
 * 
 * @param args The commands arguments
 * @param env Pointer to the environment list
 * @param last_status The last exit status (needed by the exit builtin)
 * @return The exit status of the command
 * @note Builtins receive double pointer to allow modification of the shell
 * state (in export and unset) but external commands only receive the list head
 * because they run in a separate process and can't affect the parent shell
 */
int	exec_simple_command(char **args, t_env **env, int last_status)
{
	if (!args || !args[0])
		return (0);
	if (is_builtin(args[0]))
		return (exec_builtin(args, env, last_status));
	return (exec_external(args, *env));
}
