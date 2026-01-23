/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:53:47 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:35:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exit the shell with specific status code
 * 
 * 1. If no arguments, exit with the status of the last executed command
 * 2. If the first arg is not numeric, print an error and exit with status 2
 * (no matter how many arguments there are !!)
 * 3. If the first argument is numeric but there are extra arguments, print
 * "too many arguments" and return 1 WITHOUT exiting
 * 4. If valid, convert the first argument to an 8 bit status exit code (0-255)
 * @param args Null terminated array (args[0] is "exit")
 * @param last_status The exit code of the last executed command
 * @return 1 if too many arguments, otherwise doesn't return but exits
 */
int	builtin_exit(char **args, t_shell *shell)
{
	int	argc;
	int	exit_code;

	if (shell->interactive)
		ft_putstr_fd("exit\n", 1);
	argc = ft_arrlen(args);
	if (argc == 1)
		exit(shell->exit_status);
	if (!ft_str_is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit (2);
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit_code = (unsigned char)ft_atoi(args[1]);
	exit(exit_code);
}
