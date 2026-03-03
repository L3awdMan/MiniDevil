/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:53:47 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/02 07:23:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implement the `exit` command
 * 
 * - Prints "exit" in interactive mode
 * - With no args it exits with the current exit_status
 * - With 1 numeric arg it exits with that code (casted to unsigned char)
 * - With non numeric arcs it exits with 2
 * - Too many args = error but does not exit (return 1)
 * 
 * @param args NULL terminated arg array with arg[0] = "exit"
 * @param shell SHell context (running flag set to 0 in order to trigger exit)
 * @return Exit code (0-255) or 1 if too many args (and shell keeps running)
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
