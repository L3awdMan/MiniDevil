/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:28:33 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:08:40 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "token.h"

//======================== FUNCTION: create_token ===========================
//
// PURPOSE:
//    Create a new token with given type and value
//
// RETURN:
//    t_token * - New token, or NULL on error
//
// PARAMETERS:
//    t_token_type type - Type of token (TOKEN_WORD, TOKEN_PIPE, etc.)
//    char *value       - Text content to store
//
// VARIABLES:
//    t_token *token - New token being created
//
// ALGORITHM:
//    1. Allocate memory
//    2. Set the type field: token->type = type;
//    3. Duplicate the string
//    4. Check strdup success
//          - Free the token if failed
//          - Return NULL
//    5. Initialize next pointer
//    6. Return the token

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
	token->next = NULL;
	return (token);
}

//======================== FUNCTION: free_token =============================
//
// PURPOSE:
//    Free a single token
//
// RETURN:
//    void
//
// PARAMETERS:
//    t_token *token - Token to free
//
// VARIABLES:
//    None
//
// ALGORITHM:
//    1. Check token is not NULL
//    2. Free value string
//    3. Free token

void	free_token(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}

//======================== FUNCTION: free_token_list ========================
//
// PURPOSE:
//    Free entire token list
//
// RETURN:
//    void
//
// PARAMETERS:
//    t_token *head - Head of token list
//
// VARIABLES:
//    t_token *current - Current token being freed
//    t_token *next    - Next token (saved before free)
//
// ALGORITHM:
//    1. Start at head
//    2. While current exists:
//       - Save next
//       - Free current
//       - Move to next

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

//======================== FUNCTION: add_token ==============================
//
// PURPOSE:
//    Add token to end of list
//
// RETURN:
//    void
//
// PARAMETERS:
//    t_token **head     - Pointer to head pointer
//    t_token *new_token - Token to add
//
// VARIABLES:
//    t_token *current - Pointer to traverse list
//
// ALGORITHM:
//    1. Check if new_token exists: if (!new_token) return ;
//    2. If list is empty: if (!*head)
//       - Set head to new_token: *head = new_token;
//       - Return: return ;
//    3. Traverse to end:
//       - Start at head: current = *head;
//       - Loop until last: while (current->next)
//       - Move forward: current = current->next;
//    4. Link new token: current->next = new_token;

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

//======================== FUNCTION: token_list_size ========================
//
// PURPOSE:
//    Count how many tokens are in the list
//
// RETURN:
//    int - Number of tokens
//
// PARAMETERS:
//    t_token *head - First token in list
//
// VARIABLES:
//    int count        - Counter for tokens
//    t_token *current - Used to traverse the list
//
// ALGORITHM:
//    1. Initialize counter
//    2. Start at head
//    3. Loop through list: while (current)
//       - Increment counter
//       - Move to next
//    4. Return the count

int	token_list_size(t_token *head)
{
	int		count;
	t_token	*current;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
