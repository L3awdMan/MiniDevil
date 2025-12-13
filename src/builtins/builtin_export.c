/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:35:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:18:16 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a string is a valid env variable identifier
 * 
 * Rules are:
 * - Must start with a letter or an underscore
 * - All characters that come after must be alphanumeric or underscore
 * - '+' is allowed only when it's the last character before '=' (append)
 * @param str The string to check
 * @return 1 if valid, 0 if not
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
 * @brief Print an error message for invalid export identifiers
 * @param arg The invalid argument string
 */
static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/**
 * @brief Decide on which export action to dispatch (in export_ops.c)
 * 
 * 1. Check validity of identifier
 * 2. If "+=" is found, call append function
 * 3. If "="  then call assign function
 * 4. Otherwise call no value function (export without arguments)
 * @param arg The argument string
 * @param env Double pointer to the environment list
 * @return 0 on success and 1 on error
 */
static int	export_one(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
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
 * @brief Implement the export builtin command
 * 
 * - If no arguments: print all exported variables sorted in `declare -x` format
 * - If arguments: add or update variable in the environment list
 * Continue processing the following arguments even if one is invalid
 * @param args Null terminated array (args[0] is "export")
 * @param env Double pointer to the environment list
 * @return 0 if all exports succeed, 1 if at least one error occured
 */
int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!args[1])
	{
		print_sorted_export(*env);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (export_one(args[i], env) != 0)
			status = 1;
		i++;
	}
	return (status);
}
