/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:04:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:42:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

/**
 * @brief Look up an environment variable's value by its key
 * 
 * @param env_list List head to search
 * @param key Variable name to find
 * @return Pointer to the value string or NULL if not found
 */
char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	if (!key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Update the value of an existing environment variable
 * 
 * - Search the list for a node matching the key
 * - If found, replace its value with a duplicate of the new value
 * 
 * @param env_list List head to search
 * @param key Variable name to match
 * @param value New value to set
 * @return 1 if found & updated and 0 if key not found
 */
static int	update_existing_var(t_env *env_list, char *key, char *value)
{
	t_env	*current;
	char	*new_value;

	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			new_value = ft_strdup(value);
			if (!new_value)
				return (0);
			free(current->value);
			current->value = new_value;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Create a new variable and add it to the list
 * 
 * Creates a node via create_env_node() and adds it to the list (the temporary
 * string is freed after use)
 * 
 * @param env_list Pointer to list head pointer
 * @param key Variable name
 * @param value Variable value
 * @return 1 on success and 0 on allocation failure
 */
static int	create_new_var(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;
	char	*env_string;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (0);
	env_string = ft_strjoin(tmp, value);
	free(tmp);
	if (!env_string)
		return (0);
	new_node = create_env_node(env_string);
	free(env_string);
	if (!new_node)
		return (0);
	add_env_node(env_list, new_node);
	return (1);
}

/**
 * @brief Set or create an environment variable
 * 
 * It first tries to update and existing variable, and if the key is not found,
 * it creates a new node and adds it to the list
 * 
 * @param env_list Pointer to list head pointer
 * @param key Variable name
 * @param value New value
 * @return 1 on success and 0 on allocation failure
 */
int	set_env_value(t_env **env_list, char *key, char *value)
{
	int	result;

	result = update_existing_var(*env_list, key, value);
	if (result)
		return (1);
	return (create_new_var(env_list, key, value));
}
