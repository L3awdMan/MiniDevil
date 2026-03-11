/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:52:54 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/09 07:29:14 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "env.h"
#include "libft.h"

/**
 * @brief Replace all `$` with MASK_DOLLAR
 * 
 * This is used to protect `$` in deferred expansion so that they survive the
 * tokenization and get expanded later, at execution
 * 
 * @param val String to mask
 * @return Newly allocated masked copy or NULL if fail
 */
static char	*mask_dollar(char *val)
{
	int		i;
	char	*copy;

	copy = ft_strdup(val);
	if (!copy)
		return (NULL);
	i = 0;
	while (copy[i])
	{
		if (copy[i] == '$')
			copy[i] = MASK_DOLLAR;
		i++;
	}
	return (copy);
}

/**
 * @brief Defer expansion for 1 single token
 * 
 * Unquoted `~` or `~/...` is expanded to HOME directly, while everything else
 * is kept as is (expansion happens at execution)
 * 
 * @param token Current token
 * @param prev Previous token
 * @param shell Shell context
 * @return Processed (newly allocated) string or NULL on failure
 */
static char	*defer_one(t_token *token, t_token *prev, t_shell *shell)
{
	char	*home;

	if (token->quote_type == QUOTE_SINGLE
		|| (prev && prev->type == TOKEN_HEREDOC))
		return (mask_dollar(token->value));
	if (is_dollar_quote(token))
		return (ft_strdup(""));
	if (token->quote_type != QUOTE_NONE || !token->value
		|| token->value[0] != '~')
		return (ft_strdup(token->value));
	if (token->value[1] && token->value[1] != '/')
		return (ft_strdup(token->value));
	home = get_env_value(shell->env, "HOME");
	if (!home)
		return (ft_strdup(token->value));
	if (!token->value[1])
		return (ft_strdup(home));
	return (ft_strjoin(home, token->value + 1));
}

/**
 * @brief Expand a single token
 * 
 * - Single quoted and heredoc delimis get their `$` masked
 * - Dollar quote edge casse returns an empty string
 * - Everything else goes through expand_full()
 * 
 * @param token Current token
 * @param prev Previous token
 * @param shell Shell context
 * @return Expanded string (newly allocated) or NULL if failed
 */
static char	*expand_one(t_token *token, t_token *prev, t_shell *shell)
{
	if (token->quote_type == QUOTE_SINGLE
		|| (prev && prev->type == TOKEN_HEREDOC))
		return (mask_dollar(token->value));
	if (is_dollar_quote(token))
		return (ft_strdup(""));
	return (expand_full(token->value, shell->env, token->quote_type,
			shell->exit_status));
}

/**
 * @brief Pick deferred or direct expansion
 * 
 * @param cur Current token
 * @param prev Previous token
 * @param shell Shell context
 * @param def 1 for deferred expansion and 0 for direct
 * @return Expanded or masked string (newly allocated) or NULL if failed
 */
static char	*pick_expand(t_token *cur, t_token *prev, t_shell *shell, int def)
{
	if (def)
		return (defer_one(cur, prev, shell));
	return (expand_one(cur, prev, shell));
}

/**
 * @brief Expand OR defer all TOKEN_WORD values before parsing
 * 
 * Tokens before the first && or || are expanded directly. Everything after is
 * deferred so they get expanded at execution
 * 
 * @param tokens Token list head
 * @param shell Shell context
 * @return 0 on success and -1 on allocation failure
 */
int	defer_expand_tokens(t_token *tokens, t_shell *shell)
{
	char	*expanded;
	t_token	*prev;
	int		deferred;

	prev = NULL;
	deferred = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR)
			deferred = 1;
		if (tokens->type == TOKEN_WORD)
		{
			expanded = pick_expand(tokens, prev, shell, deferred);
			if (!expanded)
				return (-1);
			free(tokens->value);
			tokens->value = expanded;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
