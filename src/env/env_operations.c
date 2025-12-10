/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:04:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:06:08 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"

// FUNCTION: get_env_value
//========================================================================
//
// PURPOSE:
//    Search environment list for a variable and return its value
//
// RETURN:
//    char * - Pointer to value string, or NULL if not found
//
// PARAMETERS:
//    t_env *env_list - Head of environment linked list
//    char *key       - Variable name to search for
//
// VARIABLES:
//    t_env *current - Pointer to traverse the list
//
// ALGORITHM:
//    1. Validate key is not NULL
//    2. Start at head: current = env_list
//    3. Loop through list:
//       - Compare current->key with search key
//       - If match found, return current->value
//       - Move to next node
//    4. If loop ends without match, return NULL

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

// FUNCTION: update_existing_var
//========================================================================
//
// PURPOSE:
//    Update value of existing environment variable
//
// RETURN:
//    int - 1 if variable found and updated, 0 if not found or error
//
// PARAMETERS:
//    t_env *env_list - Head of environment linked list
//    char *key       - Variable name to update
//    char *value     - New value to set
//
// VARIABLES:
//    t_env *current  - Pointer to traverse the list
//    char *new_value - Duplicated new value string
//
// ALGORITHM:
//    1. Start at head: current = env_list
//    2. Loop through list:
//       - Compare current->key with search key using ft_strcmp
//       - If match: duplicate new value, free old, assign new, return 1
//       - Move to next node
//    3. If loop ends without match, return 0

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

// FUNCTION: create_new_var
//========================================================================
//
// PURPOSE:
//    Create and add new environment variable to list
//
// RETURN:
//    int - 1 on success, 0 on error
//
// PARAMETERS:
//    t_env **env_list - Pointer to head of environment list
//    char *key        - Variable name
//    char *value      - Variable value
//
// VARIABLES:
//    t_env *new_node    - New node to add
//    char *env_string   - Temporary "KEY=VALUE" string
//    char *tmp          - Temporary for string building
//
// ALGORITHM:
//    1. Build "KEY=VALUE" string:
//       - Join key with "="
//       - Join result with value
//       - Free intermediate strings
//    2. Create node from env_string
//    3. Free env_string
//    4. Add node to list
//    5. Return success/failure

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

// FUNCTION: set_env_value
//========================================================================
//
// PURPOSE:
//    Update existing variable OR create new one (for export command)
//
// RETURN:
//    int - 1 on success, 0 on error
//
// PARAMETERS:
//    t_env **env_list - Pointer to head of environment list
//    char *key        - Variable name
//    char *value      - New value
//
// VARIABLES:
//    int result - Result of update attempt
//
// ALGORITHM:
//    1. Try to update existing variable
//    2. If update succeeds (returns 1), return 1
//    3. If update fails (returns 0), variable doesn't exist
//    4. Create new variable
//    5. Return result of creation

int	set_env_value(t_env **env_list, char *key, char *value)
{
	int	result;

	result = update_existing_var(*env_list, key, value);
	if (result)
		return (1);
	return (create_new_var(env_list, key, value));
}
