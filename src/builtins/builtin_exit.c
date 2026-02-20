/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:53:47 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/20 07:24:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exit the shell with specific status code
 *
 * Instead of calling exit() directly (which would skip all cleanup and
 * cause "still reachable" leaks), we set shell->running = 0 and return.
 * This lets the normal return path run: process_input frees the AST,
 * main_loop breaks, and main() frees the env list + GNL stash.
 *
 * @param args Null terminated array (args[0] is "exit")
 * @param shell Shell state (running flag is set to 0 to signal exit)
 * @return exit code, or 1 if too many arguments (without exiting)
 */
int	builtin_exit(char **args, t_shell *shell)
{
	long long	exit_code;
	int			overflow;

	if (shell->interactive)
		ft_putstr_fd("exit\n", 1);
	if (!args[1])
	{
		shell->running = 0;
		return (shell->exit_status);
	}
	exit_code = ft_atoll(args[1], &overflow);
	if (!ft_str_is_numeric(args[1]) || overflow)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->running = 0;
		return (2);
	}
	if (ft_arrlen(args) > 2)
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	shell->running = 0;
	return ((unsigned char)exit_code);
}
