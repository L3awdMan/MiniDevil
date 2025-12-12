/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 08:25:52 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 22:27:46 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a string is a valid identifier for unset
 * 
 * - Should start with a letter or underscore
 * - Next characters are alphanumeric or underscore
 * - Unlike export '+' is not allowed anywhere
 * @param str The string to check
 * @return 1 if valid, 0 if not
 */
static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Print an error message for invalid unset identifiers
 * 
 * Matches standard POSIX bash error format, although on recent versions
 * the error is silent, so we'll see if we keep this or not
 * (i'd say it's always better to notify the user of their error)
 * @param arg The invalid argument
 */
static void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/**
 * @brief Remove a node from the environment list
 * 
 * @note Uses the -1 trick with ft_strncmp() because passing -1 as a size_t
 * wraps it to SIZE_MAX and forces it to compare until the null terminator
 * (making it behave like ft_strcmp)
 * @param env Double pointer to the head of the env list
 * @param key The key to remove
 */
static void	remove_env_node(t_env **env, char *key) // could be replaced by a helper from env_operations later ?
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, -1) == 0)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				*env = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		current = current->next;
	}
}

/**
 * @brief Implement the unset builtin command
 * 
 * Remove environment variables from the list
 * - Iterate through all the arguments
 * - Validate the identifier syntax (print error if invalid)
 * - Call remove_env_node (does nothing when key not found)
 * @warning STRICT BEHAVIOR
 * This implementation uses strict identifier validation (POSIX/standard)
 * Newer bash versions (5+) may silently ignore invalid identifiers
 * (like 123 or var=val) and return 0, but this function strictly returns 1
 * and prints an error message to ensure explicit behavior
 * @param args Null terminated array (args[0] is "unset")
 * @param env Double pointer to the environment list
 * @return 0 or 1 if an invalid identifier was found
 */
int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_unset_identifier(args[i]))
		{
			print_unset_error(args[i]);
			status = 1;
		}
		else
			remove_env_node(env, args[i]);
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

static void	check_unset(t_env *env, char *key)
{
	if (get_env_value(env, key) == NULL)
		printf(GREEN "✅ %s is unset (not found)\n" RESET, key);
	else
		printf(RED "⚠️ %s still exists (unset failed)\n" RESET, key);
}

void	test_unset(t_env **env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       UNSET BUILTIN TEST               ║\n");
	printf("╚════════════════════════════════════════╝\n");

	set_env_value(env, "VAR", "isbullshit");
	set_env_value(env, "VAR1", "one");
	set_env_value(env, "VAR2", "two");
	set_env_value(env, "VAR3", "three");
	set_env_value(env, "PATH", "/usr/bin");
	printf(GRAY "Added VAR, VAR1, VAR2, VAR3, PATH\n" RESET);

	printf(YELLOW "\nTest 1: unset VAR\n" RESET);
	fflush(stdout);
	char *args1[] = {"unset", "VAR", NULL};
	builtin_unset(args1, env);
	check_unset(*env, "VAR");


	printf(YELLOW "\nTest 2: unset VAR1 VAR2 VAR3\n" RESET);
	fflush(stdout);
	char *args2[] = {"unset", "VAR1", "VAR2", "VAR3", NULL};
	builtin_unset(args2, env);
	check_unset(*env, "VAR1");
	check_unset(*env, "VAR2");
	check_unset(*env, "VAR3");


	printf(YELLOW "\nTest 3: unset NONEXISTENT\n" RESET);
	char *args3[] = {"unset", "NONEXISTENT", NULL};
	int status3 = builtin_unset(args3, env);
	if (status3 == 0)
		printf(GREEN "✅ Returned 0 (no error)\n" RESET);
	else
		printf(RED "⚠️ Returned %d (expected 0)\n" RESET, status3);


	printf(YELLOW "\nTest 4: unset PATH\n" RESET);
	char *args4[] = {"unset", "PATH", NULL};
	builtin_unset(args4, env);
	check_unset(*env, "PATH");


	printf(YELLOW "\nTest 5: unset 1_INVALID\n" RESET);
	char *args5[] = {"unset", "123_INVALID", NULL};
	printf(GRAY "(Expecting error message)\n" RESET);
	fflush(stdout);
	int status5 = builtin_unset(args5, env);
	if (status5 == 1)
		printf(GREEN "✅ returned 1 (correctly flagged as invalid)" RESET);
	else
		printf(RED "⚠️ Returned %d (expected 1)\n" RESET, status5);
}
