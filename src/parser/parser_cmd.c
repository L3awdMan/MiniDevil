/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:34:13 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/18 15:39:06 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

//==================== FUNCTION: count_word_tokens =======================
//
// PURPOSE:
//    Count how many consecutive WORD tokens exist
//
// RETURN:
//    int - Number of consecutive WORD tokens
//
// PARAMETERS:
//    t_token *tokens - Current token position
//
// ALGORITHM:
//    1. Initialize count to 0
//    2. While token exists AND is TOKEN_WORD:
//       - Increment count
//       - Move to next token
//    3. Return count

int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

//==================== FUNCTION: fill_args_array =========================
//
// PURPOSE:
//    Fill argument array with token values
//
// RETURN:
//    void
//
// PARAMETERS:
//    char **args      - Array to fill
//    t_token **tokens - Token list (advanced as we read)
//    int count        - Number of args to collect
//
// ALGORITHM:
//    1. Loop count times:
//       - Duplicate current token's value
//       - Store in args array
//       - Advance to next token
//    2. Add NULL terminator at end

static void	fill_args_array(char **args, t_token **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[count] = NULL;
}

//==================== FUNCTION: collect_args ============================
//
// PURPOSE:
//    Collect all consecutive WORD tokens into array
//
// RETURN:
//    char ** - Array of arguments, NULL on error
//
// PARAMETERS:
//    t_token **tokens - Current token (will be advanced)
//    int *argc        - Output: number of args collected
//
// ALGORITHM:
//    1. Count consecutive WORD tokens
//    2. If count is 0: return NULL (no args)
//    3. Allocate array (count + 1 for NULL)
//    4. Fill array with token values
//    5. Set argc and return array

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

//==================== FUNCTION: parse_simple_command ====================
//
// PURPOSE:
//    Parse a simple command (words only, no operators)
//
// RETURN:
//    t_ast * - Command node, NULL on error
//
// PARAMETERS:
//    t_token **tokens - Pointer to current token
//
// ALGORITHM:
//    1. Validate input
//    2. Collect all consecutive WORD tokens
//    3. If no args found: return NULL
//    4. Create command node with collected args
//    5. Return the node

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
