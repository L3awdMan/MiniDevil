/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:53:19 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/22 07:24:15 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implement the env builtin command
 *
 * Iterates through the env linked list and prints "KEY=VALUE" for every
 * variable that has a valid value (non NULL)
 * Variables with null values (created by export without value) are ignored
 * @param args Command arguments (args[0] is "env")
 * @param env Pointer to the head of the environment list
 * @return 0 on success, 127 if arguments are provided
 */
int	builtin_env(char **args, t_env *env)
{
	t_env	*current;

	if (args && args[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	current = env;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(current->value, STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}
