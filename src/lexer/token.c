/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:28:33 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:42:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"

/**
 * @brief Allocate and initialize a new token
 * 
 * The new token starts with quote_type = QUOTE_NONE, connected = 0 and
 * next = NULL
 * 
 * @param type Token type
 * @param value Text content
 * @return Newly allocated token or NULL on failure
 */
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->quote_type = QUOTE_NONE;
	token->connected = 0;
	token->next = NULL;
	return (token);
}

/**
 * @brief Free a single token and its value str
 * 
 * @param token Token to free
 */
void	free_token(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}

/**
 * @brief Free the entire token linked list
 * 
 * @param head Head of list to free
 */
void	free_token_list(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
}

/**
 * @brief Add a token to the end of a linked list
 * 
 * @param head Pointer to head pointer
 * @param new_token Token to add
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
