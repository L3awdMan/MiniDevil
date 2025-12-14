/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_conversion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:43:28 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/14 04:48:12 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a "KEY=value" string from an env node
 * 
 * @param node The environment node
 * @return A newly allocated string or NULL on fail
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
 * @brief Free the partially allocated env array (on error)
 * 
 * @param arr Array to free
 * @param count The number of elements allocated so far
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
 * @brief FIll the environment array with strings
 * 
 * @param arr Array to fill
 * @param env The environment list
 * @param size Size of the array
 * @return 1 on success, 0 on fail
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
 * @brief Convert environment linked list to char ** array
 * 
 * Create a null terminated array of "KEY=value" strings ready to be passed
 * to exeve()
 * @param env The environment linked list
 * @return Newly allocated array or NULL on fail
 * @note Caller has to free with ft_free_strarray()
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
