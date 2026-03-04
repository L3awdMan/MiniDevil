/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:34:13 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:46:29 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ast.h"
#include "libft.h"

/**
 * @brief Count consecutive word token groups
 * 
 * Connected tokens count as 1 group
 * 
 * @param tokens Token list starting position
 * @return Number of word groups
 */
int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		while (tokens && tokens->connected)
			tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

/**
 * @brief Fill argument array from consecutive word tokens
 * 
 * Concatenates the connected tokens via ft_strjoin and advances the token
 * pointer past all the consumed tokens
 * 
 * @param args Pre allocated array of size count + 1
 * @param tokens Pointer to current token pointer
 * @param count Number of word groups to collect
 */
static void	fill_args_array(char **args, t_token **tokens, int count)
{
	int		i;
	char	*word;
	char	*temp;

	i = 0;
	while (i < count)
	{
		word = ft_strdup((*tokens)->value);
		while ((*tokens)->connected && (*tokens)->next)
		{
			*tokens = (*tokens)->next;
			temp = ft_strjoin(word, (*tokens)->value);
			free(word);
			if (!temp)
				return ;
			word = temp;
		}
		args[i] = word;
		*tokens = (*tokens)->next;
		i++;
	}
	args[count] = NULL;
}

/**
 * @brief Collect consecutive word tokens into an arguments array
 * 
 * @param tokens Pointer to current token pointer
 * @param argc Number of arguments collected
 * @return NULL terminated arguments array or NULL if none
 */
char	**collect_args(t_token **tokens, int *argc)
{
	char	**args;
	int		count;

	count = count_word_tokens(*tokens);
	if (count == 0)
		return (NULL);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	fill_args_array(args, tokens, count);
	*argc = count;
	return (args);
}

/**
 * @brief Parse a simple command from consecutive word tokens
 * 
 * Collects the arguments via collect_args() and wraps them in a NODE_COMMAND
 * AST node
 * 
 * @param tokens Pointer to current token pointer
 * @return NODE_COMMAND AST node or NULL if no words were found
 */
t_ast	*parse_simple_command(t_token **tokens)
{
	char	**args;
	int		argc;

	if (!tokens || !*tokens)
		return (NULL);
	args = collect_args(tokens, &argc);
	if (!args)
		return (NULL);
	return (create_cmd_node(args, argc));
}
