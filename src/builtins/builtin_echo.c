/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:00:41 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:16:29 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if an argument is a valid -n flag
 * 
 * Valid flags must start with '-' followed by 1 or more 'n' characters
 * - Examples: "-n", "-nn", "-nnn" are valid while "-na" or "-" are not
 * @return 1 if the arg is a valid -n flag, 0 if not
 */
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implement the echo builtin command
 * 
 * This mimics the behavior of the bash echo command
 * 1. Skip all initial arguments that match the -n flag pattern
 * 2. If some flags were found, disable the newline
 * 3. Print the rest of the arguments with 1 space in between
 * 4. PRint a newline if the flag was not unset
 * 
 * @param args Null terminated array of args (arg[0] should be "echo")
 * @return Always 0 (success)
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
