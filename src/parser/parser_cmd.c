/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:34:13 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 06:50:27 by baelgadi         ###   ########.fr       */
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
 * @return 1 on success and 0 if failed midway (so that we can free what was
 * partially allocated)
 */
static int	fill_args_array(char **args, t_token **tokens, int count)
{
	int		i;
	char	*word;
	char	*temp;

	i = 0;
	while (i < count)
	{
		word = ft_strdup((*tokens)->value);
		if (!word)
			return (0);
		while ((*tokens)->connected && (*tokens)->next)
		{
			*tokens = (*tokens)->next;
			temp = ft_strjoin(word, (*tokens)->value);
			free(word);
			if (!temp)
				return (0);
			word = temp;
		}
		args[i] = word;
		*tokens = (*tokens)->next;
		i++;
	}
	return (1);
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
	args = ft_calloc(count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	if (!fill_args_array(args, tokens, count))
	{
		ft_free_strarray(args);
		return (NULL);
	}
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
