/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:35:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:19:53 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"

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
