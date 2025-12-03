/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:35:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/03 21:19:01 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

//======================== FUNCTION: init_env ===============================
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

//======================== FUNCTION: create_env_node ========================
//
// PURPOSE: Create ONE environment variable node from "KEY=VALUE" string
//
// ALGORITHM:
// 1. Malloc new t_env node
// 2. Find '=' position using ft_strchr()
//    Example: "HOME=/users/john"
//             equal_pos points to '=' at index 4
// 3. Extract key: ft_substr(env_string, 0, equal_pos - env_string)
//    Result: key = "HOME"
// 4. Extract value: ft_strdup(equal_pos + 1)
//    Result: value = "/users/john"
// 5. Initialize next = NULL, prev = NULL
// 6. Return node
//
// PARAMETERS:
//    char *env_string - One environment string "KEY=VALUE"
//
// RETURN:
//    t_env *node - New node, or NULL on error
//
// EDGE CASES:
//    - No '=' in string → invalid, return NULL
//    - Empty key (=VALUE) → invalid, return NULL
//    - Empty value (KEY=) → valid! value = ""
//    - Malloc fails → return NULL
//
// EXAMPLE:
//    Input: "PATH=/usr/bin:/bin"
//    Output: node->key = "PATH"
//            node->value = "/usr/bin:/bin"
//            node->next = NULL
//            node->prev = NULL

t_env	*create_env_node(char *env_string)
{
	t_env	*node;
	char	*equal_pos;
	int		key_len;

	node = ft_calloc(sizeof(t_env), 1);
	equal_pos = ft_strchr(env_string, '=');
	if (!equal_pos)
		return (NULL);
	key_len = equal_pos - env_string;
	(*node).key = ft_substr(env_string, 0, key_len);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(equal_pos + 1);
	if (!(*node).value)
	{
		free(node->key);
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
	// set current to head
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}
