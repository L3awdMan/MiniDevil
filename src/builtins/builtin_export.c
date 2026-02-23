/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:35:14 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 21:39:00 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+')
		{
			if (str[i + 1] == '=')
				return (1);
			else
				return (0);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	export_one(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
		if (arg[0] == '-')
			return (2);
		return (1);
	}
	if (ft_strnstr(arg, "+=", ft_strlen(arg)))
		export_append(env, arg);
	else if (ft_strchr(arg, '='))
		export_assign(env, arg);
	else
		export_no_value(env, arg);
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	status;
	int	tmp;

	if (!args[1])
		return (print_sorted_export(*env), 0);
	status = 0;
	i = 1;
	while (args[i])
	{
		tmp = export_one(args[i++], env);
		if (tmp > status)
			status = tmp;
	}
	return (status);
}
