/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:35:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 07:32:24 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a string is a vaslid env variable identifier
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
				return(0);
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

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////// TEMPORARY
//////////////////////////////////////////////////

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[90m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

static void	check_var(t_env *env, char *key, char *expected)
{
	char	*val;

	val = get_env_value(env, key);

	if (!val && !expected)
		printf(GREEN "✅ %s is unset/hidden as expected\n" RESET, key);
	else if (!val)
		printf(RED "⚠️ %s is not found (expected %s)\n" RESET, key, expected);
	else if (expected && ft_strncmp(val, expected, -1) == 0)
		printf(GREEN "✅ %s = %s\n" RESET, key, val);
	else
		printf(RED "⚠️ %s = %s (expected %s)\n" RESET, key, val, expected);
}

void	test_export(t_env **env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       EXPORT BUILTIN TEST              ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\nTest 1: export (no args)\n" RESET);
	printf(GRAY "should print sorted list with 'declare -x'\n\n" RESET);
	fflush(stdout);
	char *args1[] = {"export", NULL};
	builtin_export(args1, env);

	printf(YELLOW "\nTest 2: TEST_VAR=hamid\n" RESET);
	fflush(stdout);
	char *args2[] = {"export", "TEST_VAR=hamid", NULL};
	builtin_export(args2, env);
	check_var(*env, "TEST_VAR", "hamid");

	printf(YELLOW "\nTest 3: VAR1=one VAR2=two\n" RESET);
	fflush(stdout);
	char *args3[] = {"export", "VAR1=one", "VAR2=two", NULL};
	builtin_export(args3, env);
	check_var(*env, "VAR1", "one");
	check_var(*env, "VAR2", "two");

	printf(YELLOW "\nTest 4: 123=invalid (Should print error)\n" RESET);
	fflush(stdout);
	char *args4[] = {"export", "123=invalid", NULL};
	int status4 = builtin_export(args4, env);
	if (status4 == 1)
		printf(GREEN "✅ Returned 1 (error)\n" RESET);
	else
		printf(RED "⚠️ Returned $d (expected 1)\n" RESET);
	// verifying if it was added
	if (get_env_value(*env, "123") == NULL)
		printf(GREEN "✅ Variable 123 was not added\n" RESET);
	else
		printf(RED "⚠️ Variable 123 was added\n" RESET);

	printf(YELLOW "\nTest 5: VAR+=append\n" RESET);
	printf(GRAY "export APPEND_VAR=ham\n" RESET);
	fflush(stdout);
	set_env_value(env, "APPEND_VAR", "ham");
	printf(GRAY "export APPEND_VAR+=_id\n" RESET);
	fflush(stdout);
	char *args5[] = {"export", "APPEND_VAR+=_id", NULL};
	builtin_export(args5, env);
	// verifying now
	char *val = get_env_value(*env, "APPEND_VAR");
	if (val && ft_strncmp(val, "ham_id", -1) == 0)
		printf(GREEN "✅ %s (appended correctly)\n" RESET, val);
	else
		printf(RED "⚠️ %s (expected ham_id)\n" RESET, val);
}