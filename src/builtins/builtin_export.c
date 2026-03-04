/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:35:14 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:41:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "env.h"

/**
 * @brief Validate an export identifier (KEY or KEY= or KEY+=)
 * 
 * - Must start with alpha or `_`, followed by alnum or `_`
 * - A `+` is allowed only immediately right before `=`
 * 
 * @param str String to validate
 * @return 1 if valid and 0 otherwise
 */
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

/**
 * @brief Print export error
 * 
 * @param arg The invalid argument string
 */
static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/**
 * @brief Process an export argument
 * 
 * @param arg Argument string (KEY=value, KEY+=value or KEY)
 * @param env Pointer to env list head
 * @return 0 on success and 1 or 2 on error
 */
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

/**
 * @brief Implement the `export` command
 * 
 * - With no arguments it prints all env vars sorted by alphabetical order in
 * "declare -x" format
 * - With arguments it exports each 1 (support for =, += and no value formats)
 * 
 * @param args NULL terminated arg array with arg[0] = "export"
 * @param env Pointer to env list head (modified on export)
 * @return 0 on success or highest error code from the processed args
 */
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
