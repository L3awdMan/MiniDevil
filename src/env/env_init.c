/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:35:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:59:35 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"

//======================== FUNCTIN: init_env ===============================
//
// PURPOSE: Convert char **envp → t_env *linked list
//
// ALGORITHM:
// 1. Check if envp is NULL or empty
//    - If yes: create minimal environment (PWD, SHLVL, PATH)
// 2. Create head = NULL
// 3. FOR each string in envp[i]:
//    a. Create node from envp[i]
//    b. Add node to end of list
// 4. Handle special cases:
//    - Increment SHLVL (or set to 1 if not exists)
//    - Ensure PWD exists (set to getcwd() if not)
//    - Ensure OLDPWD exists (set to NULL if not)
// 5. Return head
//
// PARAMETERS:
//    char **envp - Environment from main()
//
// RETURN:
//    t_env *head - Pointer to first node of list
//
// EDGE CASES:
//    - envp is NULL → create minimal env
//    - envp is empty (env -i ./minishell) → create minimal env
//    - No SHLVL → set SHLVL=1
//    - SHLVL exists → increment it
//    - No PWD → set PWD=getcwd()
//
// EXAMPLE:
//  envp = ["HOME=/home/user", "PATH=/bin", NULL]
//  →
//  [HOME=/home/user] <-> [PATH=/bin] <-> [SHLVL=1] <-> [PWD=/current] <-> NULL

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	if (!envp || !envp[0])
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_env_list(&head);
			return (NULL);
		}
		add_env_node(&head, new_node);
		i++;
	}
	handle_shlvl(&head);
	ensure_pwd(&head);
	return (head);
}

//===================== FUNCTION: set_env_key_value =======================
//
// PURPOSE:
//    Extract and set key/value from "KEY=VALUE" string into node
//
// RETURN:
//    int - 1 on success, 0 on failure
//
// PARAMETERS:
//    t_env *node      - Node to populate
//    char *env_string - Full string "KEY=VALUE"
//    char *equal_pos  - Pointer to '=' character
//
// VARIABLES:
//    int key_len - Length of key part
//
// ALGORITHM:
//    1. Calculate key length: equal_pos - env_string
//    2. Extract key using ft_substr(env_string, 0, key_len)
//    3. If key extraction fails: return 0
//    4. Extract value using ft_strdup(equal_pos + 1)
//    5. If value extraction fails: free key, return 0
//    6. Return 1 (success)
//
// EXAMPLE:
//    env_string = "HOME=/home/user", equal_pos points to '='
//    → node->key = "HOME"
//    → node->value = "/home/user"
//    → returns 1

static int	set_env_key_value(t_env *node, char *env_string, char *equal_pos)
{
	int	key_len;

	key_len = equal_pos - env_string;
	node->key = ft_substr(env_string, 0, key_len);
	if (!node->key)
		return (0);
	node->value = ft_strdup(equal_pos + 1);
	if (!node->value)
	{
		free(node->key);
		return (0);
	}
	return (1);
}

//======================== FUNCTION: create_env_node ========================
//
// PURPOSE:
//    Create one environment variable node from "KEY=VALUE" string
//
// RETURN:
//    t_env * - New node, or NULL on error
//
// PARAMETERS:
//    char *env_string - Environment string "KEY=VALUE"
//
// VARIABLES:
//    t_env *node     - New node
//    char *equal_pos - Position of '='
//
// ALGORITHM:
//    1. Allocate node with ft_calloc
//    2. Find '=' with ft_strchr
//    3. If no '=': free node, return NULL
//    4. Call set_env_key_value() to extract key/value
//    5. If helper fails: free node, return NULL
//    6. Initialize next/prev to NULL
//    7. Return node
//
// EXAMPLE:
//    Input: "PATH=/usr/bin"
//    Output: node with key="PATH", value="/usr/bin"

t_env	*create_env_node(char *env_string)
{
	t_env	*node;
	char	*equal_pos;

	node = ft_calloc(sizeof(t_env), 1);
	if (!node)
		return (NULL);
	equal_pos = ft_strchr(env_string, '=');
	if (!equal_pos)
	{
		free(node);
		return (NULL);
	}
	if (!set_env_key_value(node, env_string, equal_pos))
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

// FUNCTION: add_env_node
//========================================================================
//
// PURPOSE: Add node to END of double linked list
//
// ALGORITHM:
// 1. IF list is empty (head is NULL):
//    - Set head = new_node
//    - Done!
// 2. ELSE:
//    - Find last node (traverse to end)
//    - last->next = new_node
//    - new_node->prev = last
//
// PARAMETERS:
//    t_env **head - Pointer to head pointer (can modify head)
//    t_env *new_node - Node to add
//
// RETURN:
//    void
//
// WHY **head?
//    Because if list is empty, we need to CHANGE what head points to
//
// EXAMPLE:
//    Before: [HOME=/home] <-> [PATH=/bin] <-> NULL
//    Add: [USER=john]
//    After: [HOME=/home] <-> [PATH=/bin] <-> [USER=john] <-> NULL

void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}
