/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:34:13 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:26:18 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

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
