/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:53:19 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 07:21:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

/**
 * @brief Implement the `env` command
 * 
 * Print all environment variables which have a value in "KEY=VALUE" format,
 * one per line and rejecting extra arguments.
 * 
 * @param args NULL terminated arguments array with arg[0] = "env"
 * @param env Environment list to print
 * @return 0 on success and 1 if extra arguments were given
 */
int	builtin_env(char **args, t_env *env)
{
	t_env	*current;

	if (args && args[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
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
