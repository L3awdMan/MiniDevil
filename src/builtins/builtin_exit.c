/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:53:47 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:16:56 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
