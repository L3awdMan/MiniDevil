/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:47:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:25:42 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_special_var(char *str, char **result, int exit_status)
{
	char	*value;

	if (str[1] == '?')
	{
		value = ft_itoa(exit_status);
		*result = append_str(*result, value);
		return (free(value), 2);
	}
	return (0);
}

static int	process_dollar(char *str, int i, char **result, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	int		len;

	var_name = extract_var_name(str + i + 1, &len);
	if (len == 0)
	{
		*result = append_char(*result, '$');
		free(var_name);
		return (1);
	}
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		*result = append_str(*result, var_value);
	free(var_name);
	return (len + 1);
}

static int	handle_dollar_sign(char *str, char **result, t_env *env_list,
		int exit_status)
{
	int	skip;

	skip = handle_special_var(str, result, exit_status);
	if (skip > 0)
		return (skip);
	if (is_var_char(str[1]))
		return (process_dollar(str, 0, result, env_list));
	*result = append_char(*result, '$');
	return (1);
}

char	*expand_variables(char *str, t_env *env_list, t_quote_type quote_type,
		int exit_status)
{
	char	*result;
	int		i;

	if (quote_type == QUOTE_SINGLE)
		return (ft_strdup(str));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			i += handle_dollar_sign(str + i, &result, env_list, exit_status);
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
		if (!result)
			return (NULL);
	}
	return (result);
}

int	expand_all_tokens(t_token *tokens, t_shell *shell)
{
	char	*expanded;
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (tokens->quote_type == QUOTE_SINGLE || (prev
					&& prev->type == TOKEN_HEREDOC))
				expanded = ft_strdup(tokens->value);
			else if (is_dollar_quote(tokens))
				expanded = ft_strdup("");
			else
				expanded = expand_full(tokens->value, shell->env,
						tokens->quote_type, shell->exit_status);
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
