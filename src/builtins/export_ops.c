/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 05:43:14 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/02 07:40:19 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Export a variable with no value (export KEY)
 * 
 * - If the key already exists, do nothing
 * - If it does not, create a new node with value=NULL
 * 
 * @param env Pointer to env list head
 * @param key Variable name to export
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
 * @brief Export a variable with a value (export KEY=value)
 * 
 * Split the argument at `=` to extract key and value then call set_env_value()
 * to create or update the variable
 * 
 * @param env Pointer to env list head
 * @param arg Full argument string containing `=` somewhere
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
 * @brief Build the new value for an append operation
 * 
 * If the variable already exists, concatenates old value with append_str(),
 * and if not then duplicates append_str as the new value
 * 
 * @param env Environment list for looking up existing value
 * @param key Variable name
 * @param append_str String to append
 * @return Newly allocated value
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
 * @brief Export a variable with append (export KEY+=value)
 * 
 * Splits at `+=` and builds the appended value using get_append_value()
 * then calls set_env_value() to update the variable
 * 
 * @param env Pointer to env list head
 * @param arg Full arg string containing "+="
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
