/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:35:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:41:49 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

/**
 * @brief Initialize the environment linked list from main()'s envp
 * 
 * - Iterate through envp array while creating a t_env node for each entry
 * - After building the list, increment SHLVL and ensure PWD is set
 * 
 * @param envp NULL terminated string array from main()
 * @return Head of the newly allocated environment list or NULL if env was empty
 * @note On allocation failure in the middle, frees all already created nodes
 * @warning The caller must free via free_env_list()
 */
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

/**
 * @brief Parse KEY=value and fill a pre allocated t_env node
 * 
 * - Extract the key (before =) and the value (after =) from env_string
 * - Store copies in node->key and node->value
 * - On partial failure free the already allocated key
 * 
 * @param node Pre allocated node to populate
 * @param env_string Original KEY=value string
 * @param equal_pos Pointer to the `=` char within env_string
 * @return 1 on success and 0 on allocation failure
 */
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

/**
 * @brief Create a single environment node from a KEY=value string
 * 
 * Allocate a t_env_node and populate key/value by splitting at the `=`
 * - Strings without `=` are rejected (return NULL)
 * 
 * @param env_string KEY=value string
 * @return New node with key and value set or NULL on failure
 */
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

/**
 * @brief Add a node to the end of the environment linked list
 * 
 * @param head Pointer to list head pointer
 * @param new_node Node to add
 */
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
