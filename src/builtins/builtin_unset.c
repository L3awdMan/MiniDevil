/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 08:25:52 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/23 17:19:06 by baelgadi         ###   ########.fr       */
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
 * @brief Remove a node from the environment list
 * 
 * @note Uses the -1 trick with ft_strncmp() because passing -1 as a size_t
 * wraps it to SIZE_MAX and forces it to compare until the null terminator
 * (making it behave like ft_strcmp)
 * @param env Double pointer to the head of the env list
 * @param key The key to remove
 */
// could be replaced by a helper from env_operations later ?
static void	remove_env_node(t_env **env, char *key)
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
 * @param args Null terminated array (args[0] is "unset")
 * @param env Double pointer to the environment list
 * @return 0 on success, 2 for invalid option
 */
int	builtin_unset(char **args, t_env **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1])
		{
			ft_putstr_fd("minishell: unset: -", STDERR_FILENO);
			ft_putchar_fd(args[i][1], STDERR_FILENO);
			ft_putstr_fd(": invalid option\n", STDERR_FILENO);
			return (2);
		}
		if (is_valid_unset_identifier(args[i]))
			remove_env_node(env, args[i]);
		i++;
	}
	return (0);
}
