/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:00:41 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:40:33 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

/**
 * @brief Check if a given argument is a valid -n flag
 * 
 * A valid n flag starts with `-` followed by 1 or more `n` characters
 * 
 * @param arg Argument string to check
 * @return 1 if valid and 0 if not
 */
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implement the `echo` command
 * 
 * This command prints arguments separated by spaces and supports:
 * - the -n flag and variants like -nnn
 * - multiple consecutive -n flags (consumed before print)
 * 
 * @param args NULL terminated arg array with arg[0] = "echo"
 * @return always 0
 */
int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
