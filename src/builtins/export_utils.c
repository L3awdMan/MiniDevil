/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 01:41:07 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/02 07:45:04 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Count the number of nodes in an env list
 * 
 * @param env Head of the env list
 * @return Number of nodes
 */
static int	count_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

/**
 * @brief Convert env linked list to an array of node pointers
 * 
 * Doesn't copy the nodes themselves
 * 
 * @param env Head of the env list
 * @param count Number of nodes
 * @return Array of t_env pointers or NULL
 */
static t_env	**env_to_node_array(t_env *env, int count)
{
	t_env	**arr;
	int		i;

	arr = malloc((count + 1) * sizeof(t_env *));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = env;
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

/**
 * @brief Sort an array of env node pointers by key in alphabetical order
 * 
 * Uses a bubble sort on the pointer array (doesn't modify the list)
 * 
 * @param arr Array of env node pointers
 * @param count Number of elements
 */
static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*swap;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strncmp(arr[j]->key, arr[j + 1]->key, -1) > 0)
			{
				swap = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = swap;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Print a single env var in "declare -x" format
 * 
 * @param var Env node to print
 */
static void	print_one_export(t_env *var)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(var->key, STDOUT_FILENO);
	if (var->value != NULL)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(var->value, STDOUT_FILENO);
		ft_putchar_fd('"', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

/**
 * @brief Print all env vars sorted in alphabetical order, in declare -x format
 * 
 * Used by `export` with no arguments
 * 
 * @param env Head of the env list
 */
void	print_sorted_export(t_env *env)
{
	t_env	**arr;
	int		count;
	int		i;

	count = count_env(env);
	if (count == 0)
		return ;
	arr = env_to_node_array(env, count);
	if (!arr)
		return ;
	sort_env_array(arr, count);
	i = 0;
	while (arr[i])
	{
		print_one_export(arr[i]);
		i++;
	}
	free(arr);
}
