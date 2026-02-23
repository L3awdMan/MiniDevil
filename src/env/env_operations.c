/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:04:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:19:39 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"

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

int	set_env_value(t_env **env_list, char *key, char *value)
{
	int	result;

	result = update_existing_var(*env_list, key, value);
	if (result)
		return (1);
	return (create_new_var(env_list, key, value));
}
