/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_conversion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:43:28 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:42:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

/**
 * @brief Convert an env node to a "KEY=value" string
 * 
 * Joins node->key + `=` + node->value. If value is NULL, the result is KEY=
 * with no value
 * 
 * @param node Environment node to convert
 * @return Newly allocated KEY=value string or NULL on failure
 */
static char	*create_env_string(t_env *node)
{
	char	*part;
	char	*result;

	if (!node || !node->key)
		return (NULL);
	part = ft_strjoin(node->key, "=");
	if (!part)
		return (NULL);
	if (node->value)
		result = ft_strjoin(part, node->value);
	else
		result = ft_strdup(part);
	free(part);
	return (result);
}

/**
 * @brief Free a partially filled array
 * 
 * Used for cleanup when fill_env_array() fails in the middle
 * 
 * @param arr Array to free
 * @param count Number of entries to free
 */
static void	free_incomplete_array(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief Fill an array with KEY=value strings
 * 
 * Iterate through the env list while converting each node to a string
 * - On allocation failure free all previously created strings via
 * free_incomplete_array() and return 0
 * 
 * @param arr Pre allocated array of size + 1 pointers
 * @param env List head
 * @param size Number of nodes to convert
 * @return 1 on success and 0 on allocation failure
 */
static int	fill_env_array(char **arr, t_env *env, int size)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current && i < size)
	{
		arr[i] = create_env_string(current);
		if (!arr[i])
		{
			free_incomplete_array(arr, i);
			return (0);
		}
		current = current->next;
		i++;
	}
	arr[i] = NULL;
	return (1);
}

/**
 * @brief Convert the env linked list to a NULL terminated string array
 * 
 * Builds a char** destined to execve(). Each entry is in a KEY=value string
 * - If the list is empty or NULL, it returns an array containing only NULL
 * 
 * @param env List head
 * @return NULL terminated KEY=value array or NULL on allocation failure
 */
char	**env_to_array(t_env *env)
{
	char	**arr;
	int		size;

	if (!env)
	{
		arr = ft_calloc(1, sizeof(char *));
		if (!arr)
			return (NULL);
		return (arr);
	}
	size = env_list_size(env);
	arr = ft_calloc(size + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	if (!fill_env_array(arr, env, size))
		return (NULL);
	return (arr);
}
