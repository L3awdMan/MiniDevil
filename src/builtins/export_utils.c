/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 01:41:07 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/14 05:22:46 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Count the number of nodes in the environment list
 * 
 * @param env Pointer to the head of the environment list
 * @return The total count of nodes 
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
 * @brief Create a temporary array of pointers to environment nodes
 * 
 * This allows sorting te pointer alphabetically without disruptin
 * the actual linked list structure
 * @param env Pointer to the head of the environment list
 * @param count The number of nodes
 * @return NULL terminated array of t_env pointers or NULL on fail
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
 * @brief Bubble sort the environment array alphabetically by key
 * 
 * @note Uses the -1 trick with ft_strncmp() because passing -1 as a size_t
 * wraps it to SIZE_MAX and forces it to compare until the null terminator
 * (making it behave like ft_strcmp)
 * @param arr The array of t_env pointers to sort
 * @param count The size of the array
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
 * @brief Print a single exported variable in Bash format
 * 
 * Format: declare -x KEY="VALUE"
 * - If the value is NULL (hidden) it prints declare -x KEY
 * @param var The environment variable node to print
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
 * @brief Main function to print environment variables sorted
 * 
 * 1. Count the nodes
 * 2. Create an array of pointers (temporary)
 * 3. Sort the array in alphabetical order
 * 4. Print each variable
 * 5. Free the temporary array (not the nodes)
 * @param env Pointer to the head of an environment list
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
