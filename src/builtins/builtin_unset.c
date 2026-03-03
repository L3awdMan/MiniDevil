/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 08:25:52 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/02 07:34:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validate an identifier for unset
 * 
 * Must start with alpha or `_` followed by alnum or `_` only
 * 
 * @param str String to validate
 * @return 1 if valid and 0 if not
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
 * @brief Remove an env node by key from the doubly linked list
 * 
 * Finds the node matching the key and unlinks it from the list + frees
 * 
 * @param env 
 * @param key 
 */
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
 * @brief Implement the `unset` command
 * 
 * Remove named variables from environment
 * - Invalid identifiers are skipped (silently)
 * - Options (starting with `-`) return error 2
 * 
 * @param args NULL terminated arg array with arg[0] = "unset"
 * @param env Pointer to env list head (nodes will be removed)
 * @return 0 on success and 2 if an invalid option was given
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
