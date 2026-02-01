/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 05:43:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 22:50:51 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle "export KEY" (no value)
 * 
 * If the key already exists, do nothing
 * If the key is new create a node with value = NULL (hidden)
 * @param env Double pointer to the environment list
 * @param key The variable name to export
 */
void	export_no_value(t_env **env, char *key)
{
	t_env	*new_node;
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return ;
		current = current->next;
	}
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = NULL;
	add_env_node(env, new_node);
}

/**
 * @brief Handle "export KEY=VALUE"
 * 
 * Extract key and value and use set_env_value to create or update the variable
 * @param env Double pointer to the environment list
 * @param arg The argument string that contains the assignment
 */
void	export_assign(t_env **env, char *arg)
{
	char	*key;
	char	*value;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, equal_pos - arg);
  if (!key)
    return ;
  value = ft_strdup(equal_pos + 1);
	set_env_value(env, key, value);
	free(key);
	free(value);
}

/**
 * @brief Construct the new value for the append operation
 * 
 * @param env Head of the environment list
 * @param key The variable name
 * @param append_str The string to append
 * @return A newly allocated string value
 */
static char	*get_append_value(t_env *env, char *key, char *append_str)
{
	char	*old_value;
	char	*new_value;

	old_value = get_env_value(env, key);
	if (old_value)
		new_value = ft_strjoin(old_value, append_str);
	else
		new_value = ft_strdup(append_str);
	return (new_value);
}

/**
 * @brief Handle "export KEY+=VALUE"
 * 
 * 1. Find the position of "+="
 * 2. Extract the key (excluding +)
 * 3. Calculate the new merged value
 * 4. Update the environment
 * @param env Double pointer to the environment list
 * @param arg The argument string that contains the assignment
 */
void	export_append(t_env **env, char *arg)
{
	char	*key;
	char	*value;
	char	*plus_pos;

	plus_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	key = ft_substr(arg, 0, plus_pos - arg);
	value = get_append_value(*env, key, plus_pos + 2);
	if (value)
		set_env_value(env, key, value);
	free(key);
	free(value);
}
